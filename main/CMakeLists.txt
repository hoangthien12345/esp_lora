idf_component_register(SRCS "http_sever_app.c" "dht11.c" "lora.c" "main.c" "wifi.c"
                    INCLUDE_DIRS "."
                    EMBED_FILES webpage/index.html webpage/favicon.ico webpage/style.css webpage/app.js)

