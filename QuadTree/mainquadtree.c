#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include "quadtree.h"
#include "pbPlots.h"
#include "supportLib.h"

#define part_number 20
#define size 400
#define maxspeed size/5


void QuadDraw(QuadNode *Q, ScatterPlotSettings *settings, RGBABitmapImageReference *I, RGBA *c, double wid, double hei){

    if(Q->dividido){

        //AS variáveis abaixo são para mapear a altura e a largura relativa de cada nó da quadtree nos pixels do grafico
        double xur = MapXCoordinate(Q->upright->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double yur = MapYCoordinate(Q->upright->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xul = MapXCoordinate(Q->upleft->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double yul = MapYCoordinate(Q->upleft->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xdr = MapXCoordinate(Q->downright->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double ydr = MapYCoordinate(Q->downright->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xdl = MapXCoordinate(Q->downleft->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double ydl = MapYCoordinate(Q->downleft->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xuri = MapXCoordinate(Q->upright->width_height.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double yuri = MapYCoordinate(Q->upright->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xuli = MapXCoordinate(Q->upleft->width_height.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double yuli = MapYCoordinate(Q->upleft->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xdri = MapXCoordinate(Q->downright->width_height.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double ydri = MapYCoordinate(Q->downright->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xdli = MapXCoordinate(Q->downleft->width_height.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double ydli = MapYCoordinate(Q->downleft->width_height.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xurj = MapXCoordinate(Q->upright->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double yurj = MapYCoordinate(Q->upright->Origin.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xulj = MapXCoordinate(Q->upleft->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double yulj = MapYCoordinate(Q->upleft->Origin.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xdrj = MapXCoordinate(Q->downright->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double ydrj = MapYCoordinate(Q->downright->Origin.y, 0, 400, settings->yPadding, hei - settings->yPadding);

        double xdlj = MapXCoordinate(Q->downleft->Origin.x, 0, 400, settings->xPadding, wid - settings->xPadding);
        double ydlj = MapYCoordinate(Q->downleft->Origin.y, 0, 400, settings->yPadding, hei - settings->yPadding);
        


        DrawRectangle1px(I->image, xur, yur, xuri - xurj, yurj - yuri , c);
        DrawRectangle1px(I->image, xur - 1.0, yur - 1.0, xuri - xurj, yurj - yuri , c);

        DrawRectangle1px(I->image, xul, yul, xuli - xulj, yulj - yuli , c);
        DrawRectangle1px(I->image, xul - 1.0, yul - 1.0, xuli - xulj, yulj - yuli , c);

        DrawRectangle1px(I->image, xdr, ydr, xdri - xdrj, ydrj - ydri, c);
        DrawRectangle1px(I->image, xdr - 1.0, ydr - 1.0, xdri - xdrj, ydrj - ydri, c);
                        
        DrawRectangle1px(I->image, xdl, ydl, xdli - xdlj, ydlj - ydli, c);
        DrawRectangle1px(I->image, xdl - 1.0, ydl - 1.0, xdli - xdlj, ydlj - ydli, c);


        QuadDraw(Q->downleft, settings, I, c, wid, hei);
        QuadDraw(Q->downright, settings, I, c, wid, hei);
        QuadDraw(Q->upleft, settings, I, c, wid, hei);
        QuadDraw(Q->upright, settings, I, c, wid, hei);
    }
}




int main(void){


    srand(time(0));

    Vec2 O, wh;
    O.x = 0;
    O.y = 0;
    wh.x = size;
    wh.y = size;


    QuadNode *MainQN;
    Quad *MainQ;
    List *Part_List;
    Part_List = start_List();
    MainQN = start_QuadNode(O, wh);
    MainQ = start_Quad(MainQN, Part_List);

    double x [part_number];
    double y [part_number];

    Vec2 Ini_Pos;
    Vec2 Ini_Vel;
    Ini_Vel.x = 0;
    Ini_Vel.y = 0; 


    for(int i = 0; i < part_number; i++){

        float mass = 1.16*rand();
        float rad = 1.39*rand();
        Ini_Pos.x = (double) rand()/82; //Dividido por 82 pra limitar o dominio 
        Ini_Pos.y = (double) rand()/82;


        insert_List(start_Part(i ,mass, rad, Ini_Pos, Ini_Vel), Part_List);

        x[i] = Ini_Pos.x;
        y[i] = Ini_Pos.y;

    }



    MainQN->part_list = *copy_List(MainQ->List_Part);

    quad_Check(MainQ->Raiz);
     


//Setting the plot
    wchar_t w[4] = L"dots";
    wchar_t xLabel[2] = L"x ";
    wchar_t yLabel[2] = L"y ";
    RGBA *color = CreateRGBAColor(0, 0, 0, 1.0);

    

    double wid = 1600;
    double hei = 1600;



    RGBABitmapImageReference *I =  CreateRGBABitmapImageReference();

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();

    //ScatterPlotSeries *Series = GetDefaultScatterPlotSeriesSettings();


    settings = GetDefaultScatterPlotSettings();


    settings->width = wid;
    settings->height = hei;
    settings->scatterPlotSeries = (ScatterPlotSeries**) malloc(sizeof(ScatterPlotSeries) * 1.0);
    settings->scatterPlotSeriesLength = 1.0;

    settings->scatterPlotSeries[0] = GetDefaultScatterPlotSeriesSettings();
    free(settings->scatterPlotSeries[0]->xs);
    free(settings->scatterPlotSeries[0]->ys);

    settings->autoBoundaries = false;
    settings->xMax = wh.x;
    settings->xMin = O.x;
    settings->yMax = wh.y;
    settings->yMin = O.y;

    settings->scatterPlotSeries[0]->xs = x;
    settings->scatterPlotSeries[0]->xsLength = sizeof(x)/sizeof(double);
    settings->scatterPlotSeries[0]->ys = y;
    settings->scatterPlotSeries[0]->ysLength = sizeof(y)/sizeof(double);

    
    settings->autoPadding = false;
    settings->xPadding = 5;
    settings->yPadding = 5;
    /*
    settings->xLabel = xLabel;
    settings->yLabel = yLabel;
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabelLength = wcslen(settings->yLabel);
    */
    settings->showGrid = false;
    
    

    settings->scatterPlotSeries[0]->linearInterpolation = false;
    settings->scatterPlotSeries[0]->pointType = w;
    settings->scatterPlotSeries[0]->pointTypeLength = wcslen(settings->scatterPlotSeries[0]->pointType);
    settings->scatterPlotSeries[0]->color = GetGray(1.0);

    double widh = wid - 2*settings->xPadding;
    double heih = hei - 2*settings->yPadding;
    double h = wid/hei; 
    

    DrawScatterPlotFromSettings(I, settings);

    RGBA *c =  CreateRGBAColor(0.47, 0, 1.0, 1.0);

    //desenhando a borda
    double x1 = MapXCoordinate(0, 0, 400, settings->xPadding, widh); //Mapeamento considerando o padding x no xPixelMin e Max
    double y1 = MapYCoordinate(400, 0, 400, settings->yPadding, heih); //Mapeamento considerando o padding y yPixelMin e Max

    DrawRectangle1px(I->image, x1, y1, widh, heih, c);
    DrawRectangle1px(I->image, x1 - 1.0, y1 - 1.0, widh, heih, c);
    DrawRectangle1px(I->image, x1 - 2.0, y1 - 2.0, widh, heih, c);

    //DrawRectangle1px(RGBABitmapImage *image, double x, double y, double width, double height, RGBA *color)

//A seguir, guardaremos as variáveis para o plot dos retangulos


    QuadDraw(MainQ->Raiz, settings, I, c, wid, hei);

//Fim do registro de variáveis dos retângulos






    size_t lenght;

    double *pngData = ConvertToPNG(&lenght, I->image);
    WriteToFile(pngData, lenght, "tentativafinal3.png");
	DeleteImage(I->image);
//End plotting configs        


    destroy_Quad(MainQ);




    return 0;
}