import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProcMonitorComponent } from './proc-monitor.component';

describe('MainComponent', () => {
  let component: ProcMonitorComponent;
  let fixture: ComponentFixture<ProcMonitorComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ProcMonitorComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ProcMonitorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
