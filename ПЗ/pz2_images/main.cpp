#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#include <clocale>

struct Image {
    int width;
    int height;
    std::vector<unsigned char> pixels;
};

unsigned char getPixel(const Image& img, int x, int y) {
    return img.pixels[y * img.width + x];
}

void setPixel(Image& img, int x, int y, unsigned char value) {
    img.pixels[y * img.width + x] = value;
}

bool readPGM(std::string filename, Image& image) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл: " << filename << std::endl;
        return false;
    }
    
    std::string magic;
    file >> magic;
    if (magic != "P5") {
        std::cout << "Не является P5 PGM файлом: " << filename << std::endl;
        return false;
    }
    
    int maxVal;
    file >> image.width >> image.height >> maxVal;
    file.ignore();
    
    image.pixels.resize(image.width * image.height);
    file.read((char*)image.pixels.data(), image.pixels.size());
    
    return file.good();
}

bool writePGM(std::string filename, Image image) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Не удалось создать файл: " << filename << std::endl;
        return false;
    }
    
    file << "P5\n" << image.width << " " << image.height << "\n255\n";
    file.write((char*)image.pixels.data(), image.pixels.size());
    
    return file.good();
}

void addSaltPepperNoise(Image& image, double noiseLevel) {
    srand(time(NULL));
    
    for (int i = 0; i < image.pixels.size(); i++) {
        double random = (double)rand() / RAND_MAX;
        if (random < noiseLevel) {
            double random2 = (double)rand() / RAND_MAX;
            if (random2 < 0.5) {
                image.pixels[i] = 0;
            } else {
                image.pixels[i] = 255;
            }
        }
    }
}

Image medianFilter(Image image, int windowSize) {
    Image result;
    result.width = image.width;
    result.height = image.height;
    result.pixels.resize(image.width * image.height);
    
    int half = windowSize / 2;
    
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            std::vector<unsigned char> window;
            
            for (int dy = -half; dy <= half; dy++) {
                for (int dx = -half; dx <= half; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    
                    if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
                        window.push_back(getPixel(image, nx, ny));
                    }
                }
            }
            
            std::sort(window.begin(), window.end());
            setPixel(result, x, y, window[window.size() / 2]);
        }
    }
    
    return result;
}

double calculateMSE(Image original, Image filtered) {
    double mse = 0.0;
    for (int i = 0; i < original.pixels.size(); i++) {
        double diff = original.pixels[i] - filtered.pixels[i];
        mse += diff * diff;
    }
    return mse / original.pixels.size();
}

double calculatePSNR(Image original, Image filtered) {
    double mse = calculateMSE(original, filtered);
    if (mse == 0) return 100.0;
    return 20.0 * log10(255.0 / sqrt(mse));
}

double calculateSSIM(Image original, Image filtered) {
    int windowSize = 11;
    double k1 = 0.01;
    double k2 = 0.03;
    double c1 = (k1 * 255) * (k1 * 255);
    double c2 = (k2 * 255) * (k2 * 255);
    
    double ssimSum = 0.0;
    int windowCount = 0;
    
    for (int y = windowSize/2; y < original.height - windowSize/2; y++) {
        for (int x = windowSize/2; x < original.width - windowSize/2; x++) {
            double mu1 = 0;
            double mu2 = 0;
            double sigma1 = 0;
            double sigma2 = 0;
            double sigma12 = 0;
            
            for (int dy = -windowSize/2; dy <= windowSize/2; dy++) {
                for (int dx = -windowSize/2; dx <= windowSize/2; dx++) {
                    double val1 = getPixel(original, x + dx, y + dy);
                    double val2 = getPixel(filtered, x + dx, y + dy);
                    mu1 += val1;
                    mu2 += val2;
                }
            }
            
            mu1 = mu1 / (windowSize * windowSize);
            mu2 = mu2 / (windowSize * windowSize);
            
            for (int dy = -windowSize/2; dy <= windowSize/2; dy++) {
                for (int dx = -windowSize/2; dx <= windowSize/2; dx++) {
                    double val1 = getPixel(original, x + dx, y + dy);
                    double val2 = getPixel(filtered, x + dx, y + dy);
                    sigma1 += (val1 - mu1) * (val1 - mu1);
                    sigma2 += (val2 - mu2) * (val2 - mu2);
                    sigma12 += (val1 - mu1) * (val2 - mu2);
                }
            }
            
            sigma1 = sigma1 / (windowSize * windowSize - 1);
            sigma2 = sigma2 / (windowSize * windowSize - 1);
            sigma12 = sigma12 / (windowSize * windowSize - 1);
            
            double ssim = ((2 * mu1 * mu2 + c1) * (2 * sigma12 + c2)) / 
                         ((mu1 * mu1 + mu2 * mu2 + c1) * (sigma1 + sigma2 + c2));
            
            ssimSum += ssim;
            windowCount++;
        }
    }
    
    return ssimSum / windowCount;
}

struct TestResult {
    std::string imageName;
    double noiseLevel;
    int windowSize;
    double mse;
    double psnr;
    double ssim;
};

void processAllImages(std::string inputDir, std::vector<TestResult>& results) {
    std::string filenames[] = {"Avia.pgm", "Brigada.pgm", "Siberia.pgm", "Walk.pgm", "Yakovlev.pgm"};
    double noiseLevels[] = {0.05, 0.15, 0.25};
    int windowSizes[] = {3, 5, 7};
    
    for (int i = 0; i < 5; i++) {
        std::string filename = filenames[i];
        std::string imageName = filename.substr(0, filename.find("."));
        
        std::cout << "Обработка: " << imageName << std::endl;
        
        Image original;
        if (!readPGM(inputDir + "/" + filename, original)) {
            continue;
        }
        
        for (int j = 0; j < 3; j++) {
            double noiseLevel = noiseLevels[j];
            Image noisy = original;
            addSaltPepperNoise(noisy, noiseLevel);
            
            for (int k = 0; k < 3; k++) {
                int windowSize = windowSizes[k];
                Image filtered = medianFilter(noisy, windowSize);
                
                TestResult result;
                result.imageName = imageName;
                result.noiseLevel = noiseLevel;
                result.windowSize = windowSize;
                result.mse = calculateMSE(original, filtered);
                result.psnr = calculatePSNR(original, filtered);
                result.ssim = calculateSSIM(original, filtered);
                
                results.push_back(result);
            }
        }
    }
}

void exportToCSV(std::vector<TestResult> results, std::string filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось создать CSV файл: " << filename << std::endl;
        return;
    }
    
    file << "ImageName,NoiseLevel,WindowSize,MSE,PSNR,SSIM\n";
    
    for (int i = 0; i < results.size(); i++) {
        file << results[i].imageName << ","
             << results[i].noiseLevel << ","
             << results[i].windowSize << ","
             << results[i].mse << ","
             << results[i].psnr << ","
             << results[i].ssim << "\n";
    }
    
    std::cout << "Результаты экспортированы в: " << filename << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    std::string inputDir = "pgm_images";
    std::vector<TestResult> results;
    
    std::cout << "Запуск тестирования удаления шумов с изображений..." << std::endl;
    
    processAllImages(inputDir, results);
    
    std::cout << "Всего тестов выполнено: " << results.size() << std::endl;
    
    exportToCSV(results, "results.csv");
    
    
    return 0;
}
