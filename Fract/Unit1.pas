unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  mandelbrot, ComCtrls, StdCtrls;

type
  TfrmParams = class(TForm)
    Label1: TLabel;
    TrackBar1: TTrackBar;
    Label2: TLabel;
    procedure TrackBar1Change(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmParams: TfrmParams;

implementation

{$R *.DFM}

procedure TfrmParams.TrackBar1Change(Sender: TObject);
begin
     maxIterations := TrackBar1.position;
     label2.caption := intToStr(TrackBar1.position);
end;

procedure TfrmParams.FormCreate(Sender: TObject);
begin
     TrackBar1.position := maxIterations;
end;

end.
