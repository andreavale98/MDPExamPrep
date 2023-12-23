#pragma once
#include <vector>
#include<cstdint>
#include<string>
#include <fstream>
#include<iostream>

struct BinaryImage {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
    bool  ReadFromPBM(const std::string& filename);
    BinaryImage();
    BinaryImage(const std::string& filename);
};
struct Image {
    int W;
    int H;
    std::vector<uint8_t> ImageData;
};
Image BinaryImageToImage(const BinaryImage& bimg);