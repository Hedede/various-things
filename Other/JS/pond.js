var name2;
var angle;
var dist;
var boredom;

function tooda_syooda() {
	swim(90 + 90*name2);
	if (loc_x() < 10) {
		name2 = -1;
	} else if (loc_x() > 90) {
		name2 = 1;
	}
}

function sweep() {
	dist = scan(angle);
	if (dist < 70) {
		cannon(angle, dist);
		boredom = 0;
	} else {
		angle = angle + 5*name2;
		if (angle > 360) {
			angle = 180;
		} else if (angle < 180) {
			angle = 360; 
		}
		boredom++;
	}
}

function init() {
	name2 = 1;
	dist = 0;
	angle = 270;
	boredom = 0;
	while (loc_y() < 98) {
		swim(90);
	}
	stop();
}

function descend() {
	var targy = loc_y() - 5;
	while (loc_y() > targy) {
		swim(270);
	}
	stop();
	boredom = 0;
}

function mainloop(bores) {
	while (true) {
		tooda_syooda();
		sweep();
		if (boredom > 50 && bores) {
			descend();
		}
	}
}

init();
mainloop(false);
