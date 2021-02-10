package main

import (
	"fmt"
	"log"
	"net/http"
	"strconv"

	"github.com/gorilla/websocket"

	"io/ioutil"
	"os/exec"
	"time"
	"encoding/json"
)

const (
	MEMO     = 0
	CPU      = 1
	PROCESOS = 2
)

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
				case MEMO:
					data, err := ioutil.ReadFile("/proc/mem_grupo16")
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
				case CPU:
					data, err := ioutil.ReadFile("/proc/cpu_grupo16")
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
				default:

					cmd := exec.Command("sudo", "kill", "-9", strconv.Itoa(valor))
					_, errC := cmd.Output()

					if errC != nil {
						fmt.Printf("error: %v", err)
					}

					if errW := cliente.WriteJSON("Se elimino el proceso PID: " + strconv.Itoa(valor)); errW != nil {
						log.Printf("error: %v", errW)
					}
					delete(clientes, cliente)
					cliente.Close()

					break
				}

			}
		}
		if len(clientes) == 0 {
			log.Println("Listening on :3000...")
		}

		time.Sleep(3000 * time.Millisecond)
	}
}

func main() {
	fs := http.FileServer(http.Dir("./css-Proyecto"))
	http.Handle("/", fs)
	http.HandleFunc("/ws", serverWs)
	go envioInfo()

	fmt.Println("Go WebSockets")
	log.Println("Listening on :3000...")
	err := http.ListenAndServe(":3000", nil)
	if err != nil {
		log.Fatal(err)
	}

	return
}
