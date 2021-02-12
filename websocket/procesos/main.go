package main

import (
	"fmt"
	"log"
	"net/http"
	"strconv"

	"github.com/gorilla/websocket"

	"encoding/json"
	"io/ioutil"
	"strings"
	"time"
)

const (
	PROCESOS = 2
	USUARIOS = 3
)

type Usuario struct{
	Uid string `json:"uid"`
	Username string `json:"username"`
}

var clientes = make(map[*websocket.Conn]string)

var upgrader = websocket.Upgrader{
	ReadBufferSize:  1024,
	WriteBufferSize: 1024,

	CheckOrigin: func(r *http.Request) bool { return true },
}

func serverWs(w http.ResponseWriter, r *http.Request) {
	fmt.Println(r.Host)

	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		log.Println(err)
	}
	defer ws.Close()

	reader(ws)
}

func reader(conn *websocket.Conn) {

	for {
		messageType, p, err := conn.ReadMessage()
		if err != nil {
			log.Printf("error: %v", err)
			delete(clientes, conn)
			break
		}

		clientes[conn] = string(p)
		if err := conn.WriteMessage(messageType, p); err != nil {
			log.Println(err)
			return
		}
	}

}

func envioInfo() {
	for {
		for cliente := range clientes {

			var valor, err = strconv.Atoi(clientes[cliente])
			log.Println(valor)

			if err == nil {
				switch valor {
				case PROCESOS:
					data, err := ioutil.ReadFile("/proc/procesos_grupo16")
					if err != nil {
						fmt.Println("File reading error", err)
						return
					}

					string_json := string(data)
					var data_json map[string]interface{}
					json.Unmarshal([]byte(string_json), &data_json)

					if errW := cliente.WriteJSON(data_json); errW != nil {
						log.Printf("error: %v", errW)
						cliente.Close()
						delete(clientes, cliente)
					}
					
					break
				case USUARIOS:
					data, err := ioutil.ReadFile("/etc/passwd")
					if err != nil {
						fmt.Println("File reading error", err)
						return
					}

					string_data := string(data)
					usuarios := strings.Split(string_data, "\n")

					var user Usuario

					tamanio := 0
					for _, usuario := range usuarios {
						datos := strings.Split(usuario, ":")
						if(len(datos)>1){
							tamanio++
						}
					}

					users := make([]interface{}, tamanio)

					for i, usuario := range usuarios {
						datos := strings.Split(usuario, ":")
						
						if(len(datos)>1){
							user.Uid = datos[2] 
							user.Username =  datos[0]
							users[i] = user
						}

					}

					if errW := cliente.WriteJSON(users); errW != nil {
						log.Printf("error: %v", errW)
						cliente.Close()
						delete(clientes, cliente)
					}

					break
				default:

					break
				}
			}
		}
		if len(clientes) == 0 {
			log.Println("Listening on :3001...")
		}

		time.Sleep(10000 * time.Millisecond)
	}
}

func main() {
	fs := http.FileServer(http.Dir("./css-Proyecto"))
	http.Handle("/", fs)
	http.HandleFunc("/ws", serverWs)
	go envioInfo()

	fmt.Println("Go WebSockets")
	log.Println("Listening on :3001...")
	err := http.ListenAndServe(":3001", nil)
	if err != nil {
		log.Fatal(err)
	}

	return
}
