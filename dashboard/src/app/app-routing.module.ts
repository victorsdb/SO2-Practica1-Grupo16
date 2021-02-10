import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { CpuMonitorComponent } from './components/cpu-monitor/cpu-monitor.component';
import { RamMonitorComponent } from './components/ram-monitor/ram-monitor.component';
import { ProcMonitorComponent } from './components/proc-monitor/proc-monitor.component';
import { MainComponent } from './components/main/main.component';


const routes: Routes = [
  { path: '', component: MainComponent},
  { path: 'monitor-ram', component: RamMonitorComponent },
  { path: 'monitor-cpu', component: CpuMonitorComponent },
  { path: 'procesos', component: ProcMonitorComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
