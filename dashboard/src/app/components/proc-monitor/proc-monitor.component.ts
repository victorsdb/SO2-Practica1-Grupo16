import { Component, OnInit } from '@angular/core';
import { webSocket } from 'rxjs/webSocket';

import { PageSettingsModel, SortSettingsModel, ToolbarItems } from "@syncfusion/ej2-angular-treegrid";

const subject = webSocket("ws://3.80.78.1:3001/ws");
const subject2 = webSocket("ws://3.80.78.1   :3001/ws");
const subject3 = webSocket("ws://3.80.78.1:3000/ws");

let usuarios: any[] = [];

@Component({
  selector: 'app-main',
  templateUrl: './proc-monitor.component.html',
  styleUrls: ['./proc-monitor.component.scss']
})

export class ProcMonitorComponent implements OnInit {

  public ejecutados = 0;
  public suspendidos = 0;
  public detenidos = 0;
  public zombies = 0;
  public total = 0;

  public data_procesos: any[] = [];
  public toolbarOptions: ToolbarItems[] = [];
  public sortSettings: SortSettingsModel = {};
  public pageSettings: PageSettingsModel = {};

  constructor() { }

  ngOnInit(): void {
    this.leer_usuarios();
    this.leer_procesos();
  }

  leer_procesos() {
    subject.subscribe((datos) => {

      const json_data = JSON.parse(JSON.stringify(datos));

      this.total = json_data.total;
      this.detenidos = json_data.detenidos;
      this.ejecutados = json_data.ejecutados;
      this.zombies = json_data.zombies;
      this.suspendidos = json_data.suspendidos;

      let data = json_data.procesos;

      // console.log(usuarios.includes("1000"))



      let tmp: any[] = [];
      for (const key in data) {
        if (Object.prototype.hasOwnProperty.call(data, key)) {
          const element: any = data[key];
          let username = "";

          usuarios.map((usuario) =>{
            if (usuario.uid==element["user"]) {
              username = usuario.username;
            }
          });
          const proceso = {
            pid: element["pid"],
            nombre : element["nombre"],
            user : username,
            estado : element["estado"],
            ram : element["ram"]
          };

          tmp.push(proceso);
        }
      }

      this.data_procesos = tmp;

      this.pageSettings = { pageSize: 15 };
      this.toolbarOptions = ['ExpandAll', 'CollapseAll', 'Print'];

    },
      err => console.log(err),
      () => console.log('complete')
    )
    subject.next(2);
  }

  leer_usuarios() {
    subject2.subscribe((datos) => {
      const json_data = JSON.parse(JSON.stringify(datos));

      for (const key in json_data) {
        if (Object.prototype.hasOwnProperty.call(json_data, key)) {
          const element = json_data[key];

          for (const key in element) {
            if (Object.prototype.hasOwnProperty.call(element, key)) {
              const usuario = {
                uid: element["uid"],
                username: element["username"]
              }
              
            }
          }
          usuarios.push(element)
        }
      }
      //console.log(usuarios);
    },
      err => console.log(err),
      () => console.log('complete')
    )
    subject2.next(3);
  }


  matar_proceso(proceso: string) {

    subject3.subscribe(
      msg => {
        var res = JSON.stringify(msg);
      },
      err => console.log(err),
      () => console.log('complete')
    )

    subject3.next(parseInt(proceso));
    alert("Mato el proceso con PID: " + proceso);
  }
}
