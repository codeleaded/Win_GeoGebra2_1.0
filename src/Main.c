#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"


float Function_1D(float x){
	return sinf(x);
}

TransformedView tv;

void Setup(AlxWindow* w){
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
}

void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

	Clear(BLACK);

	Vec2 wvls = { 0.0f,TransformedView_ScreenWorldY(&tv,0.0f) };
	Vec2 wvle = { 0.0f,TransformedView_ScreenWorldY(&tv,GetHeight()) };
	Vec2 whls = { TransformedView_ScreenWorldX(&tv,0.0f),0.0f };
	Vec2 whle = { TransformedView_ScreenWorldX(&tv,GetWidth()),0.0f };

	Vec2 svls = TransformedView_WorldScreenPos(&tv,wvls);
	Vec2 svle = TransformedView_WorldScreenPos(&tv,wvle);
	Vec2 shls = TransformedView_WorldScreenPos(&tv,whls);
	Vec2 shle = TransformedView_WorldScreenPos(&tv,whle);

	RenderLine(svls,svle,WHITE,1.0f);
	RenderLine(shls,shle,WHITE,1.0f);

	Vec2 PosBefore = {};
	for(int i = 0;i<GetWidth();i++){
		Vec2 Pos = { i,0.0f };
		
		float x = TransformedView_ScreenWorldX(&tv,i);
		float y = Function_1D(x);

		Pos.y = TransformedView_WorldScreenY(&tv,-y);
		
		RenderLine(PosBefore,Pos,RED,1.0f);
		PosBefore = Pos;
	}

	Vec2 p = TransformedView_ScreenWorldPos(&tv,GetMouse());
	String str = String_Format("P: X: %f, Y: %f",p.x,p.y);
	RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
	String_Free(&str);
}

void Delete(AlxWindow* w){
	
}

int main(){
    if(Create("Geogebra 1.0",2500,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}