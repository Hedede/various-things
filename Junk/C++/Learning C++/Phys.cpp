
class body {
private:
	float temperature; //kelvin
	double mass; //g
	double normalvolume; //cm^3, at 0°C
	double normaldensity; //g/cm^3, at 0°C
	double position[3];
	double velocity; 
	double acceleration; 
	double direction[3];
public:
	body(double);
	~body();
};

body::body(double bodymass) {
	mass = bodymass;

}

body::~body() {
}

class rigidbody : body {
private:
public:
	rigidbody(double);
	~rigidbody();  
};

rigidbody::rigidbody(double mass) : body(mass) {
}

rigidbody::~rigidbody() {
}

class softbody : body {
private:
public:
	softbody(double);
	~softbody();  
};

softbody::softbody(double mass) : body(mass) {
}

softbody::~softbody() {
}

int main () {
	rigidbody *rb[5];  
	rb[0] = new rigidbody (15.77);
	rb[0] = new rigidbody (12.0);
	rb[0] = new rigidbody (11.0);
	rb[0] = new rigidbody (5.0);
	rb[0] = new rigidbody (1.0);
	body bd(12.7);
}
