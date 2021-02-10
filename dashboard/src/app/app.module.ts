import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { ChartsModule } from 'ng2-charts';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';

import { RamMonitorComponent } from './components/ram-monitor/ram-monitor.component';
import { CpuMonitorComponent } from './components/cpu-monitor/cpu-monitor.component';
import { ProcMonitorComponent } from './components/proc-monitor/proc-monitor.component';


import { ToolbarItem, TreeGridModule} from '@syncfusion/ej2-angular-treegrid';

import {
  PageService,
  SortService,
  FilterService,
  ToolbarService
} from "@syncfusion/ej2-angular-treegrid";
import { MainComponent } from './components/main/main.component';

@NgModule({
  declarations: [
    AppComponent,
    RamMonitorComponent,
    CpuMonitorComponent,
    ProcMonitorComponent,
    MainComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    ChartsModule,
    TreeGridModule
  ],
  providers: [
    PageService,
    SortService,
    FilterService,
    ToolbarService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
