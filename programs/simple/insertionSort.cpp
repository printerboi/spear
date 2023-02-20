#include <iostream>


void sort(int *array){

    for(int index = 0; index < 1024; index++){
        int toSort = array[index];
        int iter = index - 1;

        while(iter >= 0 && array[iter] > toSort){
            array[iter + 1] = array[iter];
            iter = iter - 1;
        }
        array[iter + 1] = toSort;
    }
}

void print(int *array){
    std::cout << "[";


    for(int index = 0; index < 1024; index++){
        std::cout << array[index] << ",";
    }

    std::cout << "]\n";
}

int main(){
    int sortarr[] = {
            2227, 475, 3532, 1920, 6958, 1511, 9006, 9040, 6262, 7835, 5692, 4565, 3724, 2052, 3387, 3459, 555, 7854, 6377, 9225, 7877, 2613, 5312, 7577, 936, 3255, 7968, 5610, 7111, 1410, 4965, 2114, 8137, 9240, 6975, 5535, 3417, 9813, 7447, 2677, 9374, 4123, 3782, 6761, 4840, 5678, 8680, 263, 2914, 1584, 4775, 799, 4137, 3848, 9572, 1317, 3944, 2082, 7005, 4948, 7117, 9872, 861, 8756, 8550, 7054, 6194, 9074, 6715, 1749, 3000, 3204, 7608, 9122, 4224, 3951, 5886, 8115, 6565, 8296, 5573, 1354, 3697, 1704, 6632, 9097, 2896, 2092, 8084, 3866, 1743, 3682, 1806, 1895, 8911, 9567, 1321, 3534, 7167, 5691, 1686, 2379, 886, 3356, 3074, 689, 835, 8483, 8615, 8072, 2717, 3441, 2138, 3153, 8685, 2042, 4525, 9171, 9598, 378, 6546, 5079, 5714, 2141, 7627, 1367, 3263, 4602, 9752, 1119, 8507, 2805, 3448, 2239, 8977, 9940, 6011, 657, 1572, 9114, 3142, 1768, 667, 5600, 4772, 6763, 6765, 2012, 7987, 8898, 6374, 4394, 2457, 7626, 7148, 7884, 5590, 7468, 9065, 338, 8779, 276, 5377, 3626, 8099, 4419, 8578, 2415, 9217, 6394, 5348, 9309, 1654, 8604, 3088, 2086, 2991, 67, 2800, 4966, 2480, 5983, 8182, 8271, 1145, 4722, 4908, 9564, 9523, 1190, 811, 5587, 2980, 2836, 1575, 8134, 529, 6323, 2606, 582, 2408, 7723, 1270, 4535, 2514, 5190, 4009, 5619, 1693, 8495, 3472, 551, 1583, 9389, 3190, 4773, 6768, 833, 9500, 3182, 1484, 1067, 4495, 137, 8546, 3193, 5423, 3858, 9229, 8962, 9909, 982, 2425, 8051, 9179, 8969, 1226, 72, 6988, 8434, 5156, 5905, 4958, 7601, 8131, 1015, 3042, 3997, 1609, 4152, 112, 2140, 3220, 3295, 4768, 1579, 1519, 1211, 5416, 1467, 5508, 4507, 8337, 3384, 2631, 705, 7400, 680, 2989, 8440, 3974, 10, 2919, 1143, 135, 3102, 3333, 8120, 4872, 3690, 175, 8605, 9148, 8144, 8766, 2109, 1990, 9369, 725, 2639, 6305, 2382, 9350, 5681, 4443, 5741, 7407, 2025, 4802, 6136, 4166, 9606, 3112, 9288, 257, 3544, 5039, 7909, 683, 8365, 1307, 975, 8896, 5777, 4833, 807, 9083, 9950, 646, 672, 5650, 9697, 5669, 1042, 9650, 5158, 7131, 964, 4870, 2486, 4155, 3031, 2307, 9578, 9688, 1576, 5638, 5444, 2868, 6060, 2575, 9049, 1032, 6009, 3347, 8077, 2535, 2055, 3443, 3710, 1669, 4117, 1925, 3296, 5880, 5371, 2143, 7703, 3818, 5767, 5298, 1906, 9273, 5402, 7205, 9141, 908, 5515, 7699, 1454, 1377, 6703, 5808, 6287, 4737, 1245, 8776, 1318, 4556, 1928, 316, 6062, 5809, 7941, 7633, 132, 7260, 9241, 407, 1653, 5811, 6507, 2364, 4824, 7605, 5696, 8681, 2943, 7565, 5188, 4616, 8951, 6612, 4433, 9392, 5705, 2834, 7888, 7092, 7274, 8092, 2343, 8684, 595, 4884, 5406, 3304, 1218, 9525, 4295, 3985, 9289, 2464, 9058, 5269, 3256, 3158, 2285, 6413, 3290, 4576, 1252, 5605, 7383, 6355, 7543, 2189, 5623, 6863, 9779, 3351, 9285, 6250, 8314, 7657, 477, 3542, 6492, 7389, 8345, 5861, 4592, 5038, 6463, 2537, 928, 401, 9401, 3140, 466, 981, 4210, 6794, 414, 5897, 4510, 5558, 408, 1345, 2018, 4812, 2790, 7478, 4314, 9475, 9948, 340, 6701, 8715, 6637, 9031, 7679, 924, 4409, 3775, 9689, 8406, 2499, 6544, 3014, 9050, 1414, 3961, 759, 2961, 4610, 9336, 6163, 8971, 11, 1571, 3438, 3647, 764, 7506, 1442, 5331, 9664, 156, 7654, 1149, 845, 4515, 665, 3836, 8162, 1791, 53, 9425, 8130, 2566, 8074, 9189, 7060, 6689, 9311, 9656, 4992, 2376, 6607, 6653, 5323, 2356, 440, 4619, 2811, 4395, 6535, 2997, 8714, 1661, 9810, 1427, 4240, 6358, 8327, 7332, 9267, 3017, 2434, 7902, 4101, 1691, 417, 318, 193, 761, 6239, 3406, 926, 2867, 5144, 1202, 6334, 730, 2693, 6767, 7770, 8018, 3012, 1250, 7752, 8194, 6634, 7394, 2853, 5384, 3034, 3886, 9358, 1495, 6364, 5723, 3811, 4462, 7218, 5925, 3236, 3377, 8138, 6657, 6245, 5055, 9109, 2037, 568, 1305, 1498, 7275, 2700, 4693, 4628, 6613, 9812, 1341, 6949, 6883, 3855, 6726, 9796, 4905, 2466, 4960, 1390, 4269, 3253, 6480, 3960, 2954, 8879, 2502, 9150, 4460, 163, 675, 6927, 6205, 2586, 5581, 4855, 1581, 4263, 4514, 2002, 1570, 8217, 895, 9504, 5743, 8753, 3509, 3414, 5020, 8542, 8140, 2676, 2272, 8554, 5849, 6318, 6454, 3616, 6714, 9758, 6081, 989, 3016, 2760, 4648, 8142, 8100, 1435, 8218, 7792, 6400, 2937, 2664, 7454, 2634, 1847, 3343, 3832, 4044, 9283, 9162, 7776, 6468, 1734, 9166, 154, 6228, 2810, 7337, 8029, 4361, 1732, 1714, 8606, 6991, 9408, 9971, 2252, 9124, 852, 9335, 1680, 3214, 3624, 2772, 1991, 74, 5088, 6985, 224, 8190, 6431, 6686, 7927, 1301, 880, 7844, 9066, 8526, 7751, 1299, 5534, 8474, 4366, 5963, 8755, 8528, 896, 7189, 9966, 2623, 7221, 197, 8000, 2482, 733, 1618, 2523, 2073, 2830, 3566, 428, 118, 1474, 6645, 7569, 9802, 8772, 7530, 9307, 5745, 3003, 3265, 2788, 4378, 8759, 7217, 5433, 1335, 9366, 5085, 3277, 6918, 520, 4487, 3426, 1813, 1473, 2974, 6353, 5836, 6386, 5233, 3156, 9356, 80, 2214, 6534, 6352, 6778, 159, 9163, 201, 3297, 9434, 7643, 2478, 6656, 4257, 5686, 8256, 3287, 762, 8135, 6177, 4182, 6605, 9866, 5026, 3522, 6280, 1432, 3064, 9420, 7988, 4850, 2359, 3829, 4627, 6022, 8728, 7100, 8916, 3180, 3254, 2762, 5050, 5589, 7417, 1189, 1053, 8416, 6954, 4345, 7869, 1647, 4530, 8340, 55, 3205, 9761, 5664, 455, 500, 3216, 6469, 3375, 8358, 7566, 7875, 5631, 2653, 4512, 8623, 7564, 4321, 6827, 1374, 6462, 7872, 5845, 938, 2699, 1596, 1569, 3601, 9748, 4644, 6856, 3503, 3392, 4140, 1146, 567, 186, 9542, 5497, 2219, 6896, 7116, 5476, 9814, 6025, 4150, 6117, 2474, 281, 8739, 5182, 1426, 8465, 1775, 5557, 9824, 4426, 99, 1383, 2038, 2063, 8749, 4836, 4566, 3504, 2085, 2887, 8281, 9253, 4379, 7700, 1279, 6648, 5207, 8164, 5514, 9989, 2816, 314, 166, 3033, 5737, 6979, 9213, 1805, 459, 5468, 2212, 1598, 3973, 5124, 7291, 3854, 3090, 2088, 1408, 2247, 8649, 593, 9766, 7055, 363, 1852, 153, 8505, 4312, 4669, 2585, 1124, 6843, 7306, 2890, 1061, 2494, 9954, 6175, 2539, 3895, 5443, 3385, 6502, 8226, 513, 5127, 2755, 6889, 5212, 1724, 991, 5445, 1012, 5790, 5548, 5104, 8660, 1580, 1548, 7804, 5655, 7023, 7581, 7800, 6820, 2353, 5072, 6553, 2311, 7578, 6802, 6984, 862, 1091, 1483, 8015, 3217, 8348, 9084, 4882, 9023, 1891, 9194, 6229, 4285, 7542, 3926, 4819, 8800, 7706, 8293, 1909, 7760, 8907, 1057, 9993, 5544, 6957, 6176, 7211, 7041, 8321, 4675, 1113, 5834, 4351, 7091, 9232, 6409, 9230, 2579, 3316, 5454, 7962, 5719, 2443, 5697, 9345, 7015, 302, 9949, 454, 9819, 6814
    };
    sort(sortarr);

    return 0;
}