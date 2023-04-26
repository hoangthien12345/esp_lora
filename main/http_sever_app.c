

#include "http_sever_app.h"


#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static const char *TAG = "wifi";
static httpd_handle_t http_sever_handle = NULL;

extern const unsigned char index_html_start[]    asm("_binary_index_html_start");
extern const unsigned char index_html_end[]      asm("_binary_index_html_end");
extern const unsigned char favicon_ico_start[]   asm("_binary_favicon_ico_start");
extern const unsigned char favicon_ico_end[]     asm("_binary_favicon_ico_end");
extern const unsigned char style_css_start[]     asm("_binary_style_css_start");
extern const unsigned char style_css_end[]       asm("_binary_style_css_end");
extern const unsigned char app_js_start[]        asm("_binary_app_js_start");
extern const unsigned char app_js_end[]          asm("_binary_app_js_end");

/*
 *  HTML GET handler 
 */
static esp_err_t html_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "index.html requested");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
 
    return ESP_OK;
}
/**
 * favio.ico GET handler
*/
static esp_err_t favicon_ico_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "favicon.ico requested");
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, (const char *)favicon_ico_start, favicon_ico_end - favicon_ico_start);
 
    return ESP_OK;
}
/**
 * style.css GET handler
*/
static esp_err_t style_css_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "style.css requested");
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, (const char *)style_css_start, style_css_end - style_css_start);
 
    return ESP_OK;
}
/**
 * app.js get handler
*/
static esp_err_t app_js_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "app.js requested");
    httpd_resp_set_type(req, "application/javascript");
    httpd_resp_send(req, (const char *)app_js_start, app_js_end - app_js_start);
    return ESP_OK;
}

//  esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
// {
//     if (strcmp("/", req->uri) == 0) {
//         httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
//         /* Return ESP_OK to keep underlying socket open */
//         return ESP_OK;
//     } else if (strcmp("/echo", req->uri) == 0) {
//         httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
//         /* Return ESP_FAIL to close underlying socket */
//         return ESP_FAIL;
//     }
//     /* For any other URI send 404 and close socket */
//     httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
//     return ESP_FAIL;
// }



static httpd_handle_t http_sever_configure(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.core_id = 0;

	// Adjust the default priority to 1 less than the wifi application task
	config.task_priority = 4;

	// Bump up the stack size (default is 4096)
	config.stack_size = 8192;

	// Increase uri handlers
	config.max_uri_handlers = 20;

	// Increase the timeout limits
	config.recv_wait_timeout = 10;
	config.send_wait_timeout = 10;

	ESP_LOGI(TAG,
			"http_server_configure: Starting server on port: '%d' with task priority: '%d'",
			config.server_port,
			config.task_priority);
    // Start the httpd http_sever_handle
   
    if (httpd_start(&http_sever_handle, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");

        // register html handler
        httpd_uri_t index_html = {
             .uri       = "/",
             .method    = HTTP_GET,
             .handler   = html_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &index_html);
        // register fivicon_ico handler
        httpd_uri_t favicon_ico = {
             .uri       = "/favicon.ico",
             .method    = HTTP_GET,
             .handler   = favicon_ico_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &favicon_ico);
        // register style_css handler
        httpd_uri_t style_css = {
             .uri       = "/style.css",
             .method    = HTTP_GET,
             .handler   = style_css_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &style_css);
        // register app.js handler
        httpd_uri_t app_js = {
             .uri       = "/app.js",
             .method    = HTTP_GET,
             .handler   = app_js_get_handler,
             .user_ctx  = NULL
        };
        httpd_register_uri_handler(http_sever_handle, &app_js);

        return http_sever_handle;
        
    }
    else {
        ESP_LOGI(TAG, "Error starting server!");
    }
    return NULL;
    
    
}
void start_webserver(void)
{
    if(http_sever_handle == NULL)
    {
        http_sever_handle = http_sever_configure();
    }
}
void stop_webserver(void)
{
    // Stop the httpd server
   httpd_stop(http_sever_handle);
}

