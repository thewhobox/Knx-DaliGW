#include "colorhelper.h"
#include "OpenKNX.h"

uint16_t ColorHelper::getValueFromSun(uint16_t minCurr, uint16_t minDiff, uint16_t minK, uint16_t maxK)
{
    float xAchse = (minCurr * 3.14159) / minDiff;
    float yAchse = sin(xAchse);
    return (maxK - minK) * yAchse + minK;
}

void ColorHelper::rgbToXY(uint8_t in_r, uint8_t in_g, uint8_t in_b, uint16_t& x, uint16_t& y)
{
    float r = in_r / 255.0;
    float g = in_g / 255.0;
    float b = in_b / 255.0;
    r = (r   > 0.04045) ? pow((r   + 0.055) / (1.0 + 0.055), 2.4) : (r   / 12.92);
    g = (g > 0.04045) ? pow((g + 0.055) / (1.0 + 0.055), 2.4) : (g / 12.92);
    b = (b  > 0.04045) ? pow((b  + 0.055) / (1.0 + 0.055), 2.4) : (b  / 12.92);
    
	float X = r * 0.4124 + g * 0.3576 + b * 0.1805;
	float Y = r * 0.2126 + g * 0.7152 + b * 0.0722;
	float Z = r * 0.0193 + g * 0.1192 + b * 0.9505;


    float cx = X / (X + Y + Z);
    float cy = Y / (X + Y + Z);
    
    x = getBytes(cx);
    y = getBytes(cy);

    y = y + 1;
    y = y - 1;
}

void ColorHelper::hsvToRGB(uint8_t in_h, uint8_t in_s, uint8_t in_v, uint8_t& r, uint8_t& g, uint8_t& b)
{
    float h = in_h / 255.0;
    float s = in_s / 255.0;
    float v = in_v / 255.0;

    double rt = 0;
    double gt = 0;
    double bt = 0;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: rt = v, gt = t, bt = p; break;
        case 1: rt = q, gt = v, bt = p; break;
        case 2: rt = p, gt = v, bt = t; break;
        case 3: rt = p, gt = q, bt = v; break;
        case 4: rt = t, gt = p, bt = v; break;
        case 5: rt = v, gt = p, bt = q; break;
    }

	r = rt * 255;
	g = gt * 255;
	b = bt * 255;
}

void ColorHelper::kelvinToRGB(uint16_t kelvin, uint8_t& r, uint8_t& g, uint8_t& b)
{
    auto temp = kelvin / 100;

	if (temp <= 66)
	{
		r = 255;
		g = 99.4708025861 * log(temp) - 161.1195681661;

		if (temp <= 19)
			b = 0;
		else
			b = 138.5177312231 * log(temp - 10) - 305.0447927307;
	}
	else
	{
		r = 329.698727446 * pow(temp - 60, -0.1332047592);
		g = 288.1221695283 * pow(temp - 60, -0.0755148492);
		b = 255;
	}
}

void ColorHelper::xyyToRGB(uint16_t ix, uint16_t iy, uint8_t iz, uint8_t& r, uint8_t& g, uint8_t& b)
{
    float _x = getFloat(ix);
    float _y = getFloat(iy);

    //let z = 1.0 - x - y;
	//return this.colorFromXYZ((Y / y) * x, Y, (Y / y) * z);

    float y = iz / 255.0f;
    float x = _x * (y / _y);
    float z = ((1.0 - _x - _y) * y) / _y;

    float rt = x * 3.2404f + y * -1.5371f + z * -0.4985f;
    float gt = x * -0.9693f + y * 1.8760f + z * 0.0416f;
    float bt = x * 0.0556f + y * -0.2040f + z * 1.05723f;

    rt = adjust(rt);
    gt = adjust(gt);
    bt = adjust(bt);

    r = max(min(rt, 255.0f), 0.0f);
    g = max(min(gt, 255.0f), 0.0f);
    b = max(min(bt, 255.0f), 0.0f);
}

uint16_t ColorHelper::getBytes(float input)
{
	return max(min(round(input * 65534), 65534.0f), 0.0f);
}

float ColorHelper::getFloat(uint16_t input)
{
    float output = input / 65534.0f;
	return max(min(output, 1.0f), 0.0f);
}

double ColorHelper::hue2rgb(double p, double q, double t)
{
	if (t < 0) t += 1;
	if (t > 1) t -= 1;
	if (t < 1 / 6.0) return p + (q - p) * 6 * t;
	if (t < 1 / 2.0) return q;
	if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
	return p;
}

float ColorHelper::adjust(float input)
{
  if (input > 0.0031308) {
    return (1.055f * pow(input, (1.0f / 2.4f)) - 0.055f) * 255.0;
  }
  return 12.92f * input;
}