program mandel;

uses
  Forms,
  fractal in 'fractal.pas' {formMain},
  mandelbrot in 'mandelbrot.pas',
  Unit1 in 'Unit1.pas' {frmParams};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TformMain, formMain);
  Application.CreateForm(TfrmParams, frmParams);
  Application.Run;
end.
