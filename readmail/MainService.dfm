object ReadMail: TReadMail
  OldCreateOrder = False
  OnCreate = ServiceCreate
  OnDestroy = ServiceDestroy
  DisplayName = #1040#1057#1050#1059#1069' '#1095#1090#1077#1085#1080#1077' '#1087#1086#1095#1090#1099
  StartType = stManual
  AfterInstall = ServiceAfterInstall
  BeforeUninstall = ServiceBeforeUninstall
  OnContinue = ServiceContinue
  OnPause = ServicePause
  OnShutdown = ServiceShutdown
  OnStart = ServiceStart
  OnStop = ServiceStop
  Left = 192
  Top = 107
  Height = 640
  Width = 870
end
