           
           
#include "sht3xRARP.h"
#include "misc.h"
#include "ADC.h"


#define TEMP_ADC_CHANNEL 0
#define HUMI_ADC_CHANNEL 1

const int8_t temp_y[]={
	-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,
	-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,
	-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,
	-9,-8,-7,-6,-5,-4,-3,-2,-1,0,
	1,2,3,4,5,6,7,8,9,10,
	11,12,13,14,15,16,17,18,19,20,
	21,22,23,24,25,26,27,28,29,30,
	31,32,33,34,35,36,37,38,39,40,
	41,42,43,44,45,46,47,48,49,50,
	51,52,53,54,55,56,57,58,59,60,
	61,62,63,64,65,66,67,68,69,70,
	71,72,73,74,75,76,77,78,79,80,
	81,82,83,84,85,86,87,88,89,90,
	91,92,93,94,95,96,97,98,99,100,
	101,102,103,104,105,106,107,108,109,110 
};

const float temp_vol[] = {
	4.907, 4.901, 4.894, 4.888, 4.881, 4.873, 4.866, 4.858, 4.849, 4.840, 
	4.831, 4.821, 4.811, 4.800, 4.789, 4.777, 4.765, 4.752, 4.738, 4.724, 
	4.710, 4.695, 4.679, 4.663, 4.646, 4.628, 4.610, 4.591, 4.571, 4.551, 
	4.530, 4.508, 4.486, 4.463, 4.439, 4.414, 4.389, 4.363, 4.336, 4.308, 
	4.280, 4.251, 4.221, 4.191, 4.160, 4.127, 4.095, 4.062, 4.027, 3.992, 
	3.957, 3.921, 3.884, 3.847, 3.809, 3.770, 3.731, 3.692, 3.652, 3.611, 
	3.570, 3.528, 3.487, 3.444, 3.401, 3.358, 3.315, 3.272, 3.228, 3.184, 
	3.140, 3.095, 3.051, 3.007, 2.962, 2.918, 2.873, 2.829, 2.784, 2.740,
	2.696, 2.652, 2.608, 2.565, 2.521, 2.478, 2.435, 2.393, 2.351, 2.309, 
	2.267, 2.226, 2.186, 2.145, 2.106, 2.066, 2.027, 1.989, 1.951, 1.913, 
	1.876, 1.840, 1.804, 1.768, 1.733, 1.699, 1.665, 1.632, 1.599, 1.567, 
	1.535, 1.504, 1.474, 1.443, 1.414, 1.385, 1.356, 1.328, 1.301, 1.274, 
	1.248, 1.222, 1.196, 1.171, 1.147, 1.123, 1.100, 1.077, 1.054, 1.032, 
	1.011, 0.990, 0.969, 0.949, 0.929, 0.910, 0.891, 0.873, 0.855, 0.837,
	0.819, 0.803, 0.786, 0.770, 0.754, 0.738, 0.723, 0.708, 0.694, 0.680
};

const float temp_a[] = { 
	-166.169, -158.099, -150.405, -142.928, -135.504, -129.240, -122.872, -117.550, -111.949, -107.267, 
	-100.711, -97.584, -92.533, -89.601, -84.453, -81.620, -78.083, -74.769, -71.812, -68.864, 
	-66.211, -63.663, -61.182, -58.911, -56.837, -54.750, -52.802, -50.975, -49.251, -47.609, 
	-46.028, -44.710, -43.207, -41.928, -40.644, -39.327, -38.455, -37.037, -36.293, -35.497, 
	-34.329, -33.583, -32.715, -31.992, -31.108, -30.669, -30.067, -29.254, -28.542, -28.301, 
	-27.841, -27.109, -26.859,-26.324, -25.878, -25.526, -25.275, -25.134, -24.607, -24.142, 
	-24.273, -23.986, -23.207, -23.614, -23.258, -23.151, -22.925, -22.902, -22.748, -22.659, 
	-22.562, -22.531, -22.487, -22.510, -22.513, -22.487, -22.419, -22.616, -22.566, -22.681, 
	-22.745, -22.742, -23.054, -23.037, -23.208, -23.292, -23.589, -23.627, -24.060, -24.029, 
	-24.415, -24.477, -24.788, -25.169, -25.389, -25.661, -25.991, -26.385, -26.561, -27.079, 
	-27.362, -28.032, -28.091, -28.541, -29.058, -29.650, -29.889,-30.620, -30.965, -31.356, 
	-32.332, -32.865, -32.870, -34.118, -34.199, -34.976, -35.843, -36.812, -37.066, -38.202, 
	-38.551, -38.933, -40.341, -40.846, -42.521, -42.005, -43.878, -44.651, -45.490, -46.399, 
	-47.387, -48.459, -49.626, -50.895, -52.280, -51.791, -54.768, -55.159, -56.500, -57.702, 
	-58.996, -60.389, -61.891, -62.901, -64.612, -66.123, -67.390, -69.114, -70.573, -72.132,
};

//uint16_t ADC_buff[10] = 0;





float get_Humi(uint16_t _adc)
{
    float humi;
	// float vol;

    // vol = (float)_adc * 5.0 / 1024.0; 

                        

    humi = -(16.44/0.6489)+(100/0.6489)*((float)_adc/1024.0);		//r1rp
    // humi = 0.181*humi-36.481;							//rarp
	// humi[n] = -(19.7/0.54)+(100/0.54)*(Vol_Buf[m]/1024);		//rarp

    // return vol;
    return humi;

}

float get_Temp(uint16_t _adc)
{
    uint8_t  i = 0; 
    float temp;

    temp = (float)_adc * 5.0 / 1024.0; 

    if(temp < temp_vol[0])
    {
        for(i = 0; i < 150; i++)
        {
            if(temp_vol[i] >= temp && temp >= temp_vol[i+1] )
            {
                temp = temp_a[i] * (temp - temp_vol[i]) + temp_y[i];
                break;
            }
        }
    }
	else
	{
		temp = 200;
	}
	

    return temp;
}
           
