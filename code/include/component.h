class robot
{
private:
	double head_r, head_x, head_y, head_z;
	int head_t, body_t;
	double body_x, body_y, body_z, body_l, body_w, body_h, body_mul;
	double arm_x, arm_y, arm_z, arm_mul;
	double leg_x, leg_y, leg_z, leg_mul;
	;
public:
	robot();
	void drawhead();
	void drawbody();
	void drawhead1();
	void drawhead2();
	void drawbody1();
	void drawbody2();
	void drawarm();
	void drawleg();
	void headtranslate(double x, double y, double z);
	void bodytranslate(double x, double y, double z);
	void armtranslate(double x, double y, double z);
	void legtranslate(double x, double y, double z);
	;
};

