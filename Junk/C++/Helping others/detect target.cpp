/*
 * 90

177  -90 = 87
-35  -90 = -125
92   -90 = 2

  0   1
 10   0.98480775301220805936674302458952
 20   0.93969262078590838405410927732473
 30   0.86602540378443864676372317075294
 40   0.76604444311897803520239265055542
 50   0.64278760968653932632264340990726
 60   0.5
 70   0.34202014332566873304409961468226
 80   0.17364817766693034885171662676931
 90   0
100  -0.17364817766693034885171662676931
110  -0.34202014332566873304409961468226
120  -0.5
130  -0.64278760968653932632264340990726
140  -0.76604444311897803520239265055542
150  -0.86602540378443864676372317075294
160  -0.93969262078590838405410927732473
170  -0.98480775301220805936674302458952
180  -1
*/

float closestAngle = 360;
int closestId = -1;
for( int i = 0, e = NETWORK->Clients.size(); i < e; ++i ) {
	if( !NETWORK->Clients[i]->Driver ) continue;
	
	float angle = atan2( енемипоз.х - playerPos.х, енемипоз.z - playerPos.z );
	DnbPanel->Set( (NETWORK->Clients[i]->NetworkName + "y").c_str(), angle );
	
	angle = playerYaw - angle;
	if(angle > 180) angle -= 360;
	else if(angle < -180) angle += 360;
	
	angle = abs(angle);	
	
	if( angle <= closestAngle ) {
		closestAngle = angle;
		closestId = i;
	}
	DnbPanel->Set( (NETWORK->Clients[i]->NetworkName + "Y").c_str(), angle );
}
