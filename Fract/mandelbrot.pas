unit mandelbrot;

interface

uses
    Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
    Menus, ExtCtrls;

const
     maxColors = 256;
     maxSize = 4;
     maxScreenHeight = 1024;

type
    { set for bounds elements }
    SBounds = (bLeft, bTop, bRight, bBottom);

    { bounds class }
    TBounds = class
    private
           m: array[SBounds] of double;
    published
             property top: double read m[bTop] write m[bTop];
             property left: double read m[bLeft] write m[bLeft];
             property right: double read m[bRight] write m[bRight];
             property bottom: double read m[bBottom] write m[bBottom];
    public
          procedure draw(can: TCanvas);
          function screen2Fractal(sw, sh: integer; area: TBounds): TBounds;
    end;

    { base fractal type }
    TFractal = class
    protected
             rowBuff: array[0..maxScreenHeight] of double;
             area: TBounds;
             maxrow: integer;
             maxcol: integer;
             bitmap: TBitmap;
             canvas: TCanvas;
    public
          constructor Create(row, col: integer; can: TCanvas; ar: TBounds);
          procedure draw;
          procedure compute; virtual; abstract;
          procedure save(sName: string);
    published
             property bounds: TBounds read area write area;
    end;

    { mandelbrot z = z^2 + ic }
    TMandel = class(TFractal)
    public
          procedure compute; override;
    end;

var
   maxIterations: integer;

implementation

procedure TBounds.draw(can: TCanvas);
begin
     can.brush.style := bsClear;
     can.pen.mode := pmNotMask; 
     can.Rectangle( trunc(m[bLeft]),
                    trunc(m[bTop]),
                    trunc(m[bRight]),
                    trunc(m[bBottom]));
end;

function TBounds.screen2Fractal(sw, sh: integer; area: TBounds): TBounds;
var
   dx, dy: double;
begin
     dx := (area.right - area.left) / sw;
     dy := (area.top - area.bottom) / sh;

     with result do
     begin
          top := (self.m[bTop] * dy) - area.top;
          left := (self.m[bLeft] * dx) + area.left;
          right := (self.m[bRight] * dx) + area.left;
          bottom := (self.m[bBottom] * dy) - area.top;
     end;
end;

constructor TFractal.Create(row, col: integer; can: TCanvas; ar: TBounds);
begin
     maxrow := row;
     maxcol := col;
     canvas := can;
     bitmap := TBitmap.Create;
     bitmap.Height := row;
     bitmap.Width := col;
     area := ar;
end;

procedure TFractal.draw;
begin
     canvas.Draw(0,0,bitmap);
end;

procedure TFractal.save(sName: string);
begin
     bitmap.SaveToFile(sName);
end;

procedure TMandel.compute;
var
   color,row,col: integer;
   P,Q,T,modulus,deltaP,deltaQ,Xcur,Xlast,Ycur,Ylast: double;
   Xsqr, Ysqr: double;
begin
   deltaP := (area.right - area.left) / maxcol;
   deltaQ := (area.top - area.bottom) / maxrow;

   T := 0.0;
   for row := 0 to maxrow do
   begin
       rowBuff[row] := area.bottom + T;
       T := T + deltaQ;
   end;

   T := 0.0;
   for col := 0 to maxcol do
   begin
        P := area.left + T;
        for row := 0 to maxrow do
        begin
             Q := rowBuff[row];
             Xlast := 0.0;
             Ylast := 0.0;
             modulus := 0.0;
             color := 0;
             Xsqr := 0.0;
             Ysqr := 0.0;
             repeat
                   inc(color);
                   Xcur := Xsqr - Ysqr + P;
                   Ycur := Xlast * Ylast;
                   Ycur := Ycur + Ycur + Q;
                   Xsqr := Xcur * Xcur;
                   Ysqr := Ycur * Ycur;
                   Xlast := Xcur;
                   Ylast := Ycur;
                   modulus := Xsqr + Ysqr;
             until ((modulus >= maxSize) or (color >= maxIterations));

             //dec(color);
             color := maxColors * color div maxIterations;
             bitmap.canvas.pixels[col, row] := RGB(256-color, 256-color div 2, color);
        end;
        T := T + deltaP;
        if (col mod 20) = 0 then
           self.draw;
   end;
end;

begin
     maxIterations := 16;
end.
