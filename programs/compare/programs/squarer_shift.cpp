#include <iostream>
#include "../../helper/randomFiller.cpp"


int main(){
    int n = 1000;
    int elements[] = {1249, 1288, 14864, 16470, 13712, 11788, 17312, 5255, 5248, 4385, 14760, 13844, 9887, 14272, 2115, 15000, 17097, 1238, 3650, 13921, 8469, 10988, 16852, 4800, 9947, 14460, 1261, 16415, 14503, 5881, 12690, 12732, 576, 2037, 708, 3506, 4064, 11761, 8424, 4292, 4870, 17352, 16474, 6559, 9300, 5315, 8175, 4813, 12582, 16244, 9693, 7189, 15109, 672, 13157, 15403, 14539, 10530, 4359, 5893, 14295, 17873, 4963, 10489, 1191, 1829, 117, 5293, 942, 17664, 127, 10756, 6939, 15905, 12909, 2888, 12031, 5181, 81, 2782, 2013, 522, 8199, 6940, 4090, 11099, 850, 11150, 15835, 5889, 278, 12364, 14767, 8056, 1327, 3051, 3860, 15245, 5575, 1062, 11835, 9851, 691, 16732, 11466, 7718, 2500, 13875, 9111, 7688, 17079, 1569, 4018, 8322, 2713, 12380, 1361, 16247, 10253, 13257, 4631, 5553, 5853, 42, 17987, 5693, 14900, 5479, 17796, 10328, 341, 15155, 4876, 17567, 6265, 7256, 747, 3305, 7997, 5671, 3446, 13155, 1232, 14920, 17868, 13372, 894, 5465, 16808, 9298, 17107, 5799, 16512, 12056, 621, 10781, 9395, 9455, 14414, 2008, 1654, 17503, 7941, 3712, 12250, 15678, 3493, 16508, 12426, 16175, 8704, 1377, 14312, 12329, 2839, 4624, 938, 3851, 11117, 594, 6288, 11719, 14445, 3177, 1098, 4737, 10402, 13318, 15160, 10400, 6540, 8135, 13642, 13402, 11354, 16773, 12908, 5980, 8528, 12576, 14803, 12942, 17787, 4339, 11857, 5190, 9296, 1915, 1601, 1465, 5369, 13668, 6225, 15786, 9208, 14587, 1345, 16420, 4575, 16499, 15287, 7026, 53, 11114, 3763, 5216, 4451, 3355, 7539, 11273, 13727, 13610, 2428, 3094, 8268, 7936, 4727, 12078, 9739, 6241, 3246, 182, 17739, 13016, 2891, 14178, 7869, 14273, 4531, 17283, 8534, 6803, 14316, 14977, 6419, 13129, 8696, 12041, 7090, 872, 10378, 1088, 16371, 14572, 8702, 12124, 16655, 5773, 3714, 16691, 4244, 6821, 6342, 2838, 14364, 8549, 4143, 8672, 2814, 17613, 15592, 10978, 9750, 5885, 11358, 17633, 5296, 1697, 4962, 11247, 1051, 1368, 13076, 13834, 3477, 13100, 17526, 1662, 4774, 9470, 4942, 13481, 10315, 16252, 7616, 2625, 11972, 5836, 9896, 14054, 15868, 5661, 10314, 583, 3048, 17304, 14902, 13371, 15819, 10657, 7070, 9679, 16208, 8415, 598, 7113, 3772, 14798, 9749, 15133, 3511, 10005, 2099, 17259, 458, 13450, 6214, 13195, 1056, 11389, 9152, 3195, 13428, 11160, 2468, 11801, 12994, 9571, 6536, 13266, 16524, 8569, 1780, 4160, 12228, 7467, 9760, 1775, 6602, 12673, 16740, 6011, 5067, 15524, 2603, 10684, 4390, 17666, 14421, 1189, 9401, 14198, 8195, 4643, 14243, 15395, 4132, 16304, 16122, 16074, 12198, 17738, 17876, 5179, 1214, 3265, 5645, 9136, 6760, 10061, 10012, 9284, 12220, 13265, 8054, 4889, 6495, 4616, 14478, 3462, 12189, 17279, 13107, 13636, 14549, 13621, 12943, 9755, 4979, 16281, 11006, 15551, 6061, 5236, 15552, 12399, 11859, 759, 6592, 8550, 8132, 2197, 5074, 10175, 6571, 7720, 8236, 8847, 12745, 8209, 2201, 9836, 11770, 2984, 6851, 17851, 7320, 17921, 15342, 3115, 15876, 16190, 16200, 16156, 5840, 4370, 2665, 16141, 1155, 1403, 9261, 5368, 17282, 5113, 14875, 16779, 12175, 6922, 13230, 5924, 12430, 9502, 10674, 13513, 1468, 16189, 12766, 16917, 14024, 10667, 3081, 16337, 718, 3005, 17743, 7610, 1366, 9474, 302, 13438, 9439, 3262, 5570, 11544, 12390, 3460, 9844, 14935, 2514, 8639, 9788, 12739, 10982, 17749, 11884, 5271, 662, 4826, 17183, 14179, 17130, 6574, 10653, 2377, 1756, 16133, 14074, 7532, 14080, 11025, 2460, 8491, 9747, 7337, 7905, 2960, 8714, 9032, 7569, 1614, 16758, 14869, 2342, 14598, 740, 17770, 11950, 1396, 2986, 15296, 8817, 15669, 16525, 16040, 15426, 8475, 2246, 13422, 5056, 3572, 236, 16819, 220, 5744, 9024, 14241, 6627, 5172, 8749, 4224, 13547, 4920, 1917, 2953, 2280, 853, 20, 3066, 12713, 17871, 11020, 6117, 15802, 8040, 4346, 13158, 4447, 14259, 2591, 7646, 5962, 16672, 13379, 8535, 11436, 4590, 14351, 3055, 16268, 12074, 7611, 9916, 12886, 17769, 2996, 8129, 2457, 14957, 15495, 9643, 13311, 11795, 15919, 7571, 6477, 10024, 12818, 5994, 9542, 3523, 13586, 4437, 2287, 944, 2672, 9192, 16969, 11721, 1539, 14600, 10627, 7182, 317, 14433, 2737, 7071, 12548, 7519, 12224, 8720, 2919, 14371, 2310, 9088, 1742, 12993, 13281, 1057, 2901, 13734, 4726, 16991, 15326, 841, 1250, 4259, 11742, 13489, 14695, 10427, 12774, 16802, 1902, 3037, 6282, 9393, 17285, 468, 5025, 17311, 3461, 3818, 16048, 5241, 11699, 17346, 4611, 14560, 4565, 12741, 7733, 9397, 11206, 13835, 10298, 3623, 500, 4426, 6475, 160, 16885, 12401, 17754, 11133, 8351, 2789, 10857, 16082, 17437, 3393, 11604, 3635, 3970, 11144, 11506, 11716, 15512, 16401, 1220, 10195, 105, 12262, 3396, 1733, 15581, 13178, 4566, 5875, 2564, 16487, 2521, 15660, 11989, 3751, 523, 8358, 7287, 13518, 11035, 2774, 1563, 13984, 8898, 17529, 15359, 13962, 570, 1969, 15117, 778, 5399, 14365, 5385, 9666, 14215, 12107, 1933, 7993, 7874, 15702, 7483, 7417, 3685, 13051, 12551, 16488, 1386, 1129, 4869, 17017, 3830, 2633, 16891, 14378, 5333, 4556, 9254, 3519, 12200, 5141, 2688, 14476, 1299, 7175, 1413, 8171, 9957, 6441, 7572, 2899, 15406, 15241, 3014, 15389, 17314, 9952, 5203, 7641, 3319, 8496, 17137, 4029, 13578, 7096, 14685, 13175, 15153, 13440, 11204, 7740, 13609, 4187, 12100, 11772, 17251, 14790, 3639, 17088, 7169, 11678, 1671, 15936, 1119, 5501, 12254, 16436, 11456, 9346, 12988, 12814, 3087, 5495, 14060, 3482, 2516, 3933, 4693, 12624, 14448, 15228, 6024, 9200, 15604, 8291, 16589, 11548, 2658, 17267, 4324, 9161, 601, 16361, 12217, 12865, 13276, 13986, 3791, 5639, 16608, 2575, 11301, 10480, 4701, 12136, 9260, 2753, 11782, 13663, 1823, 10255, 17819, 10052, 14837, 1484, 706, 13745, 9688, 4445, 4551, 7575, 15694, 16639, 10214, 1693, 17212, 1081, 16375, 12444, 4261, 773, 15, 4413, 8655, 2439, 7001, 2168, 13770, 8045, 10274, 762, 4333, 13777, 11396, 8020, 9335, 450, 4854, 7738, 17423, 7697, 17950, 3849, 7250, 4526, 227, 12195, 10018, 4312, 4275, 16471, 2518, 10553, 877, 2240, 13115, 8730, 9249, 5149, 7781, 10444, 3443, 7864, 4392, 151, 4236, 17692, 6205, 10268, 6555, 14052, 15548, 14822, 17724, 10915, 13590, 7850, 17533, 8399, 8497, 3504, 11817, 1391, 6299, 11147, 14016, 148, 13728, 10600, 736, 3476, 1582, 1521, 398, 10308, 12635, 8311, 8712, 12578, 9766, 5349, 1260, 661, 10201, 13815, 5048, 14310, 16444, 93, 10819, 2260, 16851, 7540, 7546, 2422, 10329, 12299, 3975, 981, 11467, 15758, 11139, 864, 15693, 10724, 159, 16804, 5347, 11213, 4182, 5598, 3731, 9205, 14196, 7240, 11999, 12180, 10456, 4577, 1721, 6959, 1194, 41, 7560, 17208, 4890, 15088, 17307, 5660, 8202, 8976, 17990, 11205, 1913, 7542, 16271, 2167, 9082, 7947, 6212, 7006};

    int before = 333;
    int square = before * before;

    for(int i=0; i < n; i++){
        elements[i] = elements[i] + elements[i];
    }

    return 0;
}