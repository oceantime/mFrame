#include "WmConvert.h"
#include <ctype.h>
#include <map>
#include <sstream>
#include <cstdlib>
#include <unordered_map>

namespace wmcanvas {



    WmColorRGBA StrValueToColorRGBA(const char *value) {
        static std::unordered_map<std::string, WmColorRGBA> colorMap;
        if (colorMap.empty()) {
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "black", {{0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "silver", {{192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gray", {{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "white", {{255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "maroon", {{128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "red", {{255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "purple", {{128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "fuchsia", {{255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "green", {{0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lime", {{0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "olive", {{128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "yellow", {{255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "navy", {{0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blue", {{0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "teal", {{0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aqua", {{0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aliceblue", {{240.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "antiquewhite",
                    {{250.0f / 255, 235.0f / 255, 215.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aqua", {{0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aquamarine", {{127.0f / 255, 255.0f / 255, 212.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "azure", {{240.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "beige", {{245.0f / 255, 245.0f / 255, 220.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "bisque", {{255.0f / 255, 228.0f / 255, 196.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "black", {{0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blanchedalmond",
                    {{255.0f / 255, 235.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blue", {{0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blueviolet", {{138.0f / 255, 43.0f / 255, 226.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "brown", {{165.0f / 255, 42.0f / 255, 42.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "burlywood", {{222.0f / 255, 184.0f / 255, 135.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cadetblue", {{95.0f / 255, 158.0f / 255, 160.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "chartreuse", {{127.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "chocolate", {{210.0f / 255, 105.0f / 255, 30.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "coral", {{255.0f / 255, 127.0f / 255, 80.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cornflowerblue",
                    {{100.0f / 255, 149.0f / 255, 237.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cornsilk", {{255.0f / 255, 248.0f / 255, 220.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "crimson", {{220.0f / 255, 20.0f / 255, 60.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cyan", {{0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkblue", {{0.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkcyan", {{0.0f / 255, 139.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgoldenrod",
                    {{184.0f / 255, 134.0f / 255, 11.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgray", {{169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgreen", {{0.0f / 255, 100.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgrey", {{169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkkhaki", {{189.0f / 255, 183.0f / 255, 107.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkmagenta", {{139.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkolivegreen",
                    {{85.0f / 255, 107.0f / 255, 47.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkorange", {{255.0f / 255, 140.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkorchid", {{153.0f / 255, 50.0f / 255, 204.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkred", {{139.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darksalmon", {{233.0f / 255, 150.0f / 255, 122.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkseagreen",
                    {{143.0f / 255, 188.0f / 255, 143.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkslateblue", {{72.0f / 255, 61.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkslategray", {{47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkslategrey", {{47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkturquoise", {{0.0f / 255, 206.0f / 255, 209.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkviolet", {{148.0f / 255, 0.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "deeppink", {{255.0f / 255, 20.0f / 255, 147.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "deepskyblue", {{0.0f / 255, 191.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "dimgray", {{105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "dimgrey", {{105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "dodgerblue", {{30.0f / 255, 144.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "firebrick", {{178.0f / 255, 34.0f / 255, 34.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "floralwhite", {{255.0f / 255, 250.0f / 255, 240.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "forestgreen", {{34.0f / 255, 139.0f / 255, 34.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "fuchsia", {{255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gainsboro", {{220.0f / 255, 220.0f / 255, 220.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "ghostwhite", {{248.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gold", {{255.0f / 255, 215.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "goldenrod", {{218.0f / 255, 165.0f / 255, 32.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gray", {{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "green", {{0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "greenyellow", {{173.0f / 255, 255.0f / 255, 47.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "grey", {{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "honeydew", {{240.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "hotpink", {{255.0f / 255, 105.0f / 255, 180.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "indianred", {{205.0f / 255, 92.0f / 255, 92.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "indigo", {{75.0f / 255, 0.0f / 255, 130.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "ivory", {{255.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "khaki", {{240.0f / 255, 230.0f / 255, 140.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lavender", {{230.0f / 255, 230.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lavenderblush",
                    {{255.0f / 255, 240.0f / 255, 245.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lawngreen", {{124.0f / 255, 252.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lemonchiffon",
                    {{255.0f / 255, 250.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightblue", {{173.0f / 255, 216.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightcoral", {{240.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightcyan", {{224.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgoldenrodyellow",
                    {{250.0f / 255, 250.0f / 255, 210.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgray", {{211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgreen", {{144.0f / 255, 238.0f / 255, 144.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgrey", {{211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightpink", {{255.0f / 255, 182.0f / 255, 193.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightsalmon", {{255.0f / 255, 160.0f / 255, 122.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightseagreen",
                    {{32.0f / 255, 178.0f / 255, 170.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightskyblue",
                    {{135.0f / 255, 206.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightslategray",
                    {{119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightslategrey",
                    {{119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightsteelblue",
                    {{176.0f / 255, 196.0f / 255, 222.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightyellow", {{255.0f / 255, 255.0f / 255, 224.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lime", {{0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "limegreen", {{50.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "linen", {{250.0f / 255, 240.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "magenta", {{255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "maroon", {{128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumaquamarine",
                    {{102.0f / 255, 205.0f / 255, 170.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumblue", {{0.0f / 255, 0.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumorchid", {{186.0f / 255, 85.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumpurple",
                    {{147.0f / 255, 112.0f / 255, 219.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumseagreen",
                    {{60.0f / 255, 179.0f / 255, 113.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumslateblue",
                    {{123.0f / 255, 104.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumspringgreen",
                    {{0.0f / 255, 250.0f / 255, 154.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumturquoise",
                    {{72.0f / 255, 209.0f / 255, 204.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumvioletred",
                    {{199.0f / 255, 21.0f / 255, 133.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "midnightblue", {{25.0f / 255, 25.0f / 255, 112.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mintcream", {{245.0f / 255, 255.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mistyrose", {{255.0f / 255, 228.0f / 255, 225.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "moccasin", {{255.0f / 255, 228.0f / 255, 181.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "navajowhite", {{255.0f / 255, 222.0f / 255, 173.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "navy", {{0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "oldlace", {{253.0f / 255, 245.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "olive", {{128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "olivedrab", {{107.0f / 255, 142.0f / 255, 35.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "orange", {{255.0f / 255, 165.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "orangered", {{255.0f / 255, 69.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "orchid", {{218.0f / 255, 112.0f / 255, 214.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "palegoldenrod",
                    {{238.0f / 255, 232.0f / 255, 170.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "palegreen", {{152.0f / 255, 251.0f / 255, 152.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "paleturquoise",
                    {{175.0f / 255, 238.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "palevioletred",
                    {{219.0f / 255, 112.0f / 255, 147.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "papayawhip", {{255.0f / 255, 239.0f / 255, 213.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "peachpuff", {{255.0f / 255, 218.0f / 255, 185.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "peru", {{205.0f / 255, 133.0f / 255, 63.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "pink", {{255.0f / 255, 192.0f / 255, 203.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "plum", {{221.0f / 255, 160.0f / 255, 221.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "powderblue", {{176.0f / 255, 224.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "purple", {{128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "red", {{255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "rosybrown", {{188.0f / 255, 143.0f / 255, 143.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "royalblue", {{65.0f / 255, 105.0f / 255, 225.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "saddlebrown", {{139.0f / 255, 69.0f / 255, 19.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "salmon", {{250.0f / 255, 128.0f / 255, 114.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "sandybrown", {{244.0f / 255, 164.0f / 255, 96.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "seagreen", {{46.0f / 255, 139.0f / 255, 87.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "seashell", {{255.0f / 255, 245.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "sienna", {{160.0f / 255, 82.0f / 255, 45.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "silver", {{192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "skyblue", {{135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "slateblue", {{106.0f / 255, 90.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "slategray", {{112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "slategrey", {{112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "snow", {{255.0f / 255, 250.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "springgreen", {{0.0f / 255, 255.0f / 255, 127.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "steelblue", {{70.0f / 255, 130.0f / 255, 180.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "tan", {{210.0f / 255, 180.0f / 255, 140.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "teal", {{0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "thistle", {{216.0f / 255, 191.0f / 255, 216.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "tomato", {{255.0f / 255, 99.0f / 255, 71.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "turquoise", {{64.0f / 255, 224.0f / 255, 208.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "violet", {{238.0f / 255, 130.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "wheat", {{245.0f / 255, 222.0f / 255, 179.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "white", {{255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "whitesmoke", {{245.0f / 255, 245.0f / 255, 245.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "yellow", {{255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "yellowgreen", {{154.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "transparent", {{0, 0, 0, 0.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "transparent_white", {{1.f, 1.f, 1.f, 0.f}}));
        }


        WmColorRGBA c = {{0.0f, 0.0f, 0.0f, 1.0f}};
        if (value == nullptr) {
            return c;
        }


        if (value[0] == '#') {
            int length = (int) strlen(value);
            char str[] = "ffffff";

            if (length == 4) {
                str[0] = str[1] = value[3];
                str[2] = str[3] = value[2];
                str[4] = str[5] = value[1];
                unsigned int hex =
                        (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
                c.rgba = {(hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                          (hex >> 16) / 255.0f, 1.0};
            } else if (length == 7) { // #ff00ff format
                str[0] = value[5];
                str[1] = value[6];
                str[2] = value[3];
                str[3] = value[4];
                str[4] = value[1];
                str[5] = value[2];
                unsigned int hex =
                        (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
                c.rgba = {(hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                          (hex >> 16) / 255.0f, 1.0};
            }
        } else {
            // 按名称query
        //     std::string colorVal = value;
        //     colorVal.erase(std::remove(colorVal.begin(), colorVal.end(), ' '), colorVal.end());
        //     std::transform(colorVal.begin(), colorVal.end(), colorVal.begin(),
        //                    ::tolower);
        //     value = colorVal.c_str();

            auto iter = colorMap.find(value);
            if (iter != colorMap.end()) {
                return iter->second;
            }

            // TODO rgba 格式
            int length = (int) strlen(value);
            if (strncmp(value, "rgb(", 4) == 0 || strncmp(value, "rgba(", 5) == 0) {
                int current = 0;
                for (int i = 4; i < length && current < 4; i++) {
                    if (current == 3) {
                        // If we have an alpha component, copy the rest of the wide
                        // string into a char array and use atof() to parse it.
                        char alpha[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                        for (int j = 0; i + j < length - 1 && j < 7; j++) {
                            alpha[j] = value[i + j];
                        }
                        double d = atof(alpha);
                        if (d > 1) {
                            d = 1;
                        }
                        c.components[current] = d;
                        current++;
                    } else if (isdigit(value[i])) {
                        c.components[current] =
                                (c.components[current] * 10 + (value[i] - '0'));
                    } else if (value[i] == ',' || value[i] == ')') {
                        c.components[current] /= 255.0f;
                        current++;
                    }
                }
            }
        }
        return c;
    }



    WmColorRGBA StrValueToColorRGBALegacy(const char *value) {
        static std::unordered_map<std::string, WmColorRGBA> colorMap;
        if (colorMap.empty()) {
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "black", {{0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "silver", {{192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gray", {{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "white", {{255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "maroon", {{128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "red", {{255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "purple", {{128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "fuchsia", {{255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "green", {{0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lime", {{0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "olive", {{128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "yellow", {{255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "navy", {{0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blue", {{0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "teal", {{0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aqua", {{0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aliceblue", {{240.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "antiquewhite",
                    {{250.0f / 255, 235.0f / 255, 215.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aqua", {{0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "aquamarine", {{127.0f / 255, 255.0f / 255, 212.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "azure", {{240.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "beige", {{245.0f / 255, 245.0f / 255, 220.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "bisque", {{255.0f / 255, 228.0f / 255, 196.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "black", {{0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blanchedalmond",
                    {{255.0f / 255, 235.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blue", {{0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "blueviolet", {{138.0f / 255, 43.0f / 255, 226.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "brown", {{165.0f / 255, 42.0f / 255, 42.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "burlywood", {{222.0f / 255, 184.0f / 255, 135.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cadetblue", {{95.0f / 255, 158.0f / 255, 160.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "chartreuse", {{127.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "chocolate", {{210.0f / 255, 105.0f / 255, 30.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "coral", {{255.0f / 255, 127.0f / 255, 80.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cornflowerblue",
                    {{100.0f / 255, 149.0f / 255, 237.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cornsilk", {{255.0f / 255, 248.0f / 255, 220.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "crimson", {{220.0f / 255, 20.0f / 255, 60.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "cyan", {{0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkblue", {{0.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkcyan", {{0.0f / 255, 139.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgoldenrod",
                    {{184.0f / 255, 134.0f / 255, 11.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgray", {{169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgreen", {{0.0f / 255, 100.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkgrey", {{169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkkhaki", {{189.0f / 255, 183.0f / 255, 107.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkmagenta", {{139.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkolivegreen",
                    {{85.0f / 255, 107.0f / 255, 47.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkorange", {{255.0f / 255, 140.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkorchid", {{153.0f / 255, 50.0f / 255, 204.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkred", {{139.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darksalmon", {{233.0f / 255, 150.0f / 255, 122.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkseagreen",
                    {{143.0f / 255, 188.0f / 255, 143.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkslateblue", {{72.0f / 255, 61.0f / 255, 139.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkslategray", {{47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkslategrey", {{47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkturquoise", {{0.0f / 255, 206.0f / 255, 209.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "darkviolet", {{148.0f / 255, 0.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "deeppink", {{255.0f / 255, 20.0f / 255, 147.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "deepskyblue", {{0.0f / 255, 191.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "dimgray", {{105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "dimgrey", {{105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "dodgerblue", {{30.0f / 255, 144.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "firebrick", {{178.0f / 255, 34.0f / 255, 34.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "floralwhite", {{255.0f / 255, 250.0f / 255, 240.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "forestgreen", {{34.0f / 255, 139.0f / 255, 34.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "fuchsia", {{255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gainsboro", {{220.0f / 255, 220.0f / 255, 220.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "ghostwhite", {{248.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gold", {{255.0f / 255, 215.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "goldenrod", {{218.0f / 255, 165.0f / 255, 32.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "gray", {{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "green", {{0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "greenyellow", {{173.0f / 255, 255.0f / 255, 47.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "grey", {{128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "honeydew", {{240.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "hotpink", {{255.0f / 255, 105.0f / 255, 180.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "indianred", {{205.0f / 255, 92.0f / 255, 92.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "indigo", {{75.0f / 255, 0.0f / 255, 130.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "ivory", {{255.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "khaki", {{240.0f / 255, 230.0f / 255, 140.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lavender", {{230.0f / 255, 230.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lavenderblush",
                    {{255.0f / 255, 240.0f / 255, 245.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lawngreen", {{124.0f / 255, 252.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lemonchiffon",
                    {{255.0f / 255, 250.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightblue", {{173.0f / 255, 216.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightcoral", {{240.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightcyan", {{224.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgoldenrodyellow",
                    {{250.0f / 255, 250.0f / 255, 210.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgray", {{211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgreen", {{144.0f / 255, 238.0f / 255, 144.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightgrey", {{211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightpink", {{255.0f / 255, 182.0f / 255, 193.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightsalmon", {{255.0f / 255, 160.0f / 255, 122.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightseagreen",
                    {{32.0f / 255, 178.0f / 255, 170.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightskyblue",
                    {{135.0f / 255, 206.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightslategray",
                    {{119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightslategrey",
                    {{119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightsteelblue",
                    {{176.0f / 255, 196.0f / 255, 222.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lightyellow", {{255.0f / 255, 255.0f / 255, 224.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "lime", {{0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "limegreen", {{50.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "linen", {{250.0f / 255, 240.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "magenta", {{255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "maroon", {{128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumaquamarine",
                    {{102.0f / 255, 205.0f / 255, 170.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumblue", {{0.0f / 255, 0.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumorchid", {{186.0f / 255, 85.0f / 255, 211.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumpurple",
                    {{147.0f / 255, 112.0f / 255, 219.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumseagreen",
                    {{60.0f / 255, 179.0f / 255, 113.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumslateblue",
                    {{123.0f / 255, 104.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumspringgreen",
                    {{0.0f / 255, 250.0f / 255, 154.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumturquoise",
                    {{72.0f / 255, 209.0f / 255, 204.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mediumvioletred",
                    {{199.0f / 255, 21.0f / 255, 133.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "midnightblue", {{25.0f / 255, 25.0f / 255, 112.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mintcream", {{245.0f / 255, 255.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "mistyrose", {{255.0f / 255, 228.0f / 255, 225.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "moccasin", {{255.0f / 255, 228.0f / 255, 181.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "navajowhite", {{255.0f / 255, 222.0f / 255, 173.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "navy", {{0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "oldlace", {{253.0f / 255, 245.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "olive", {{128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "olivedrab", {{107.0f / 255, 142.0f / 255, 35.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "orange", {{255.0f / 255, 165.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "orangered", {{255.0f / 255, 69.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "orchid", {{218.0f / 255, 112.0f / 255, 214.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "palegoldenrod",
                    {{238.0f / 255, 232.0f / 255, 170.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "palegreen", {{152.0f / 255, 251.0f / 255, 152.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "paleturquoise",
                    {{175.0f / 255, 238.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "palevioletred",
                    {{219.0f / 255, 112.0f / 255, 147.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "papayawhip", {{255.0f / 255, 239.0f / 255, 213.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "peachpuff", {{255.0f / 255, 218.0f / 255, 185.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "peru", {{205.0f / 255, 133.0f / 255, 63.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "pink", {{255.0f / 255, 192.0f / 255, 203.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "plum", {{221.0f / 255, 160.0f / 255, 221.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "powderblue", {{176.0f / 255, 224.0f / 255, 230.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "purple", {{128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "red", {{255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "rosybrown", {{188.0f / 255, 143.0f / 255, 143.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "royalblue", {{65.0f / 255, 105.0f / 255, 225.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "saddlebrown", {{139.0f / 255, 69.0f / 255, 19.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "salmon", {{250.0f / 255, 128.0f / 255, 114.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "sandybrown", {{244.0f / 255, 164.0f / 255, 96.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "seagreen", {{46.0f / 255, 139.0f / 255, 87.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "seashell", {{255.0f / 255, 245.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "sienna", {{160.0f / 255, 82.0f / 255, 45.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "silver", {{192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "skyblue", {{135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "slateblue", {{106.0f / 255, 90.0f / 255, 205.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "slategray", {{112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "slategrey", {{112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "snow", {{255.0f / 255, 250.0f / 255, 250.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "springgreen", {{0.0f / 255, 255.0f / 255, 127.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "steelblue", {{70.0f / 255, 130.0f / 255, 180.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "tan", {{210.0f / 255, 180.0f / 255, 140.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "teal", {{0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "thistle", {{216.0f / 255, 191.0f / 255, 216.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "tomato", {{255.0f / 255, 99.0f / 255, 71.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "turquoise", {{64.0f / 255, 224.0f / 255, 208.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "violet", {{238.0f / 255, 130.0f / 255, 238.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "wheat", {{245.0f / 255, 222.0f / 255, 179.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "white", {{255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "whitesmoke", {{245.0f / 255, 245.0f / 255, 245.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "yellow", {{255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "yellowgreen", {{154.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "transparent", {{0, 0, 0, 0.0f}}));
            colorMap.insert(std::pair<std::string, WmColorRGBA>(
                    "transparent_white", {{1.f, 1.f, 1.f, 0.f}}));
        }


        WmColorRGBA c = {{0.0f, 0.0f, 0.0f, 1.0f}};
        if (value == nullptr) {
            return c;
        }


        // 按名称query
        std::string colorVal = value;
        colorVal.erase(std::remove(colorVal.begin(), colorVal.end(), ' '), colorVal.end());
        std::transform(colorVal.begin(), colorVal.end(), colorVal.begin(),
                       ::tolower);
        value = colorVal.c_str();

        auto iter = colorMap.find(value);
        if (iter != colorMap.end()) {
            return iter->second;
        }

        int length = (int) strlen(value);
        char str[] = "ffffff";

        // #f0f format
        if (length == 4) {
            str[0] = str[1] = value[3];
            str[2] = str[3] = value[2];
            str[4] = str[5] = value[1];
            unsigned int hex =
                    (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
            c.rgba = {(hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                      (hex >> 16) / 255.0f, 1.0};
        }
            // #ff00ff format
        else if (length == 7) {
            str[0] = value[5];
            str[1] = value[6];
            str[2] = value[3];
            str[3] = value[4];
            str[4] = value[1];
            str[5] = value[2];
            unsigned int hex =
                    (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
            c.rgba = {(hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                      (hex >> 16) / 255.0f, 1.0};
        } else { // assume rgb(255,0,255) or rgba(255,0,255,0.5) format
            if (strncmp(value, "rgb(", 4) == 0 || strncmp(value, "rgba(", 5) == 0) {
                int current = 0;
                for (int i = 4; i < length && current < 4; i++) {
                    if (current == 3) {
                        // If we have an alpha component, copy the rest of the wide
                        // string into a char array and use atof() to parse it.
                        char alpha[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                        for (int j = 0; i + j < length - 1 && j < 7; j++) {
                            alpha[j] = value[i + j];
                        }
                        double d = atof(alpha);
                        if (d > 1) {
                            d = 1;
                        }
                        c.components[current] = d;
                        current++;
                    } else if (isdigit(value[i])) {
                        c.components[current] =
                                (c.components[current] * 10 + (value[i] - '0'));
                    } else if (value[i] == ',' || value[i] == ')') {
                        c.components[current] /= 255.0f;
                        current++;
                    }
                }
            }
        }
        return c;
    }


    std::string ColorToString(const WmColorRGBA &color) {
        std::ostringstream ss;
        ss << "rgba(" << (int) (color.rgba.r * 255)
           << "," << (int) (color.rgba.g * 255) << "," << (int) (color.rgba.b * 255)
           << "," << (int) color.rgba.a << ")";
        return ss.str();
    }

    /**
     * int (RGBA) convert to WmColorRGBA
     */
    WmColorRGBA IntValueToColorRGBA(int value) {
        if (value == 0) {
            return GColorTransparent;
        }

        WmColorRGBA c;
        c.rgba = {((value & 0xff000000) >> 24) / 255.0f, ((value & 0xff0000) >> 16) / 255.0f,
                  ((value & 0xff00) >> 8) / 255.0f, (value & 0xff) / 255.0f };
        return c;
    }

}
