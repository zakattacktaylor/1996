unit fractal;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Menus, ExtCtrls, mandelbrot;

type
  TformMain = class(TForm)
    MainMenu1: TMainMenu;
    Fractal1: TMenuItem;
    Draw1: TMenuItem;
    menuSave: TMenuItem;
    SaveDialog1: TSaveDialog;
    ScrollBox1: TScrollBox;
    screen: TPaintBox;
    Params1: TMenuItem;
    procedure Draw1Click(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure menuSaveClick(Sender: TObject);
    procedure ScrollBox1Resize(Sender: TObject);
    procedure screenMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure screenMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure screenPaint(Sender: TObject);
    procedure Params1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  formMain: TformMain;
  mandel: TMandel;
  s2: TBounds;
  area: TBounds;

procedure screen2Fract(var sx, sy: double);

implementation

uses Unit1;

{$R *.DFM}

procedure screen2Fract(var sx, sy: double);
var
   dx, dy: double;
begin
     dx := abs(area.right - area.left) / 640;
     dy := abs(area.top - area.bottom) / 480;

     sx := sx * dx;
     sy := sy * dy;

     sx := sx + area.left;
     sy := area.top - sy;
end;

procedure TformMain.Draw1Click(Sender: TObject);
var
   x1, y1, x2, y2: double;
begin
     x1 := s2.left;
     y1 := s2.top;
     screen2Fract(x1,y1);

     x2 := s2.right;
     y2 := s2.bottom;
     screen2Fract(x2,y2);

     area.left := x1;
     area.top := y1;
     area.right := x2;
     area.bottom := y2;

     mandel.compute;
     mandel.draw;
end;

procedure TformMain.FormPaint(Sender: TObject);
begin
     if mandel <> nil then
        mandel.draw;
end;

procedure TformMain.menuSaveClick(Sender: TObject);
begin
     SaveDialog1.Execute;
     mandel.save(SaveDialog1.FileName);     
end;

procedure TformMain.ScrollBox1Resize(Sender: TObject);
begin
     if mandel <> nil then
        mandel.draw;
end;

procedure TformMain.screenMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
     s2.top := Y;
     s2.left := X;
end;

procedure TformMain.screenMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
     if ssLeft in shift then
     begin
          s2.right := X;
          s2.bottom := Y;
          if mandel <> nil then
             mandel.draw;
          s2.draw(screen.canvas);
     end;
end;

procedure TformMain.FormCreate(Sender: TObject);
begin
     s2 := TBounds.create;
     s2.left := 0;
     s2.top := 0;
     s2.right := 640;
     s2.bottom := 480;
     area := TBounds.Create;
     area.top := 1.5;
     area.left := -1.5;
     area.bottom := -1.5;
     area.right := 1.5;
     mandel := TMandel.Create(480,640,screen.canvas,area);
end;

procedure TformMain.screenPaint(Sender: TObject);
begin
     if mandel <> nil then
        mandel.draw;
end;

procedure TformMain.Params1Click(Sender: TObject);
begin
     frmParams.show;
end;

end.
