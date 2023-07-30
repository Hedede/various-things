double m_tab[] = { 0, 0.221, 0.366 };

void sub_40196C(TForm2* caller)
{
	double Tob  = Sysutils::StrToFloat( caller->ctrl1->GetText() );
	double Kob  = Sysutils::StrToFloat( caller->ctrl0->GetText() );
	double tau  = Sysutils::StrToFloat( caller->ctrl2->GetText() );
	double step = Sysutils::StrToFloat( caller->ctrl3->GetText() );

	for ( i = 0; i < 3; ++i ) {
		m = m_tab[i];

		double pX = -1.0;
		double pY;
		for ( double w = step; w < PI; w = step + w ) {
			double v2 = exp(m * w) * Kob;
			double v3 = pow(1.0 - Tob * m * w, 2.0);
			Amp = v2 / sqrt(v3 + Tob * Tob * w * w);

			double fz1 = atan(Tob * w / (1.0 - Tob * m * w)) + tau * w;
			double fz2 = atan(m) + fz1 - PI/2;

			double v4 = sqrt(m * m + 1.0);
			pX = sin(fz2) * v4 / Amp;

			double v5 = sqrt(m * m + 1.0) * w;
			pY = (sin(fz2) * m + cos(fz2)) * v5 / Amp;

			v8 = pY * Kob - m * w * w;
			if ( v8 < 0.0 )
				break;
			double x,y;
			if ( m == 0.0 ) {
				x = pX * Kob;
				y = pY * Kob;
				caller->chart1->AddXY(x, y, 0x0000FF);
			}
			if ( m == 0.221 ) {
				x = pX * Kob - m * w;
				y = pY * Kob - m * w;
				caller->chart0->AddXY(x, y, 0xFF0000);
			}
			if ( m == 0.366 ) {
				x = pX * Kob - m * w;
				y = pY * Kob - m * w;
				caller->chart2->AddXY(x, y, 0x008000);
			}
		}
	}
}
