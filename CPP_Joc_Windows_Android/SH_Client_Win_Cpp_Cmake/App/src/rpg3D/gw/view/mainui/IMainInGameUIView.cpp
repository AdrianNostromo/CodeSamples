#include "IMainInGameUIView.h"

using namespace rpg3D;

int IMainInGameUIView::GetNewID() {
	static int COUNTER = 0;
	
	int id = COUNTER;
	COUNTER++;

	return id;
}

int IMainInGameUIView::ID = GetNewID();
