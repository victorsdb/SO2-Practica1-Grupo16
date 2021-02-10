import { Component, OnInit } from '@angular/core';
import { webSocket } from "rxjs/webSocket";

import { PageSettingsModel, SortSettingsModel, ToolbarItems } from "@syncfusion/ej2-angular-treegrid";


import { Proceso } from './proceso';

const subject = webSocket("ws://localhost:3000/ws");
const subject2 = webSocket("ws://localhost:3000/ws");

@Component({
  selector: 'app-main',
  templateUrl: './main.component.html',
  styleUrls: ['./main.component.scss']
})

export class MainComponent implements OnInit {

  public ejecutados = 0;
  public suspendidos = 0;
  public detenidos = 0;
  public zombies = 0;
  public total = 0;

  public data_procesos: Proceso[] = [];
  public toolbarOptions: ToolbarItems[] = [];
  public sortSettings: SortSettingsModel = {};
  public pageSettings: PageSettingsModel = {};

  constructor() { }

  ngOnInit(): void {
    this.leer_procesos();
  }

  leer_procesos() {
    subject.subscribe((datos) => {

      const json_data = JSON.parse(JSON.stringify(datos))

      this.total = json_data.total;
      this.detenidos = json_data.detenidos;
      this.ejecutados = json_data.ejecutados;
      this.zombies = json_data.zombies;
      this.suspendidos = json_data.suspendidos;

      let data = json_data.procesos;

      let tmp: Proceso[] = [];
      for (const key in data) {
        if (Object.prototype.hasOwnProperty.call(data, key)) {
          const element: Proceso = data[key];
          tmp.push(element);
        }
      }

      this.data_procesos = tmp;
      
      this.sortSettings = {
        columns: [
          {
            field: "nombre",
            direction: "Ascending"
          },
        ]
      };

      this.pageSettings = { pageSize: 15 };
      this.toolbarOptions = ['ExpandAll', 'CollapseAll', 'Print'];

    },
      err => console.log(err),
      () => console.log('complete')
    )
    subject.next(2);
  }

  matar_proceso(proceso: string) {

    subject2.subscribe(
      msg => {
        var res = JSON.stringify(msg);
      },
      err => console.log(err),
      () => console.log('complete')
    )

    subject2.next(parseInt(proceso));
    alert("Mato el proceso con PID: " + proceso);
  }
}
