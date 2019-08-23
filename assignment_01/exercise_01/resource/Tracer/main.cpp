#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef unsigned char byte;

struct Color
{
	byte r, g, b, a;
};

struct Vec3
{
	float x, y, z;

	Vec3 operator- (const Vec3& other)
	{
		return Vec3{x - other.x, y - other.y, z - other.z};
	}

	Vec3 operator+ (const Vec3& other) 
	{
		return Vec3{x + other.x, y + other.y, z + other.z};
	}

	static float dot(const Vec3& a, const Vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
};

struct Hit
{
	Vec3 position;
	Vec3 distance;
	Vec3 normal;
	Color color;
}

struct Sphere
{
	Vec3 position;
	float radius;
	Color color;

	bool intersect(Vec3 origin, Vec3 dir)
	{
	 	Vec3 oc = origin - position;
	    float a = Vec3::dot(dir, dir);
	    float b = 2.0 * Vec3::dot(oc, dir);
	    float c = Vec3::dot(oc, oc) - radius * radius;
	    float discriminant = b*b - 4*a*c;
	    return (discriminant>0);
	}
};

struct Image
{
	int width;
	int height;
	Color* data;	

	Image(int width, int height)
	{
		this->width = width;
		this->height = height;
		data = new Color[width * height];
	}

	~Image()
	{
		delete[] data;
	}

	void setPixel(int x, int y, Color c)
	{
		data[y * width + x]	= c;
	}
};

void write_to_disk(Image* img, std::string path)
{
	stbi_write_png(path.c_str(), img->width, img->height, 4, (void*)img->data, 0);		
}

int main()
{
	std::cout << "Hello Tracer" << std::endl;

	int width = 600;
	int height = 400;
	
	Vec3 cam = {0, 0, -10};
	Sphere s = {Vec3{0, 0, 0}, 9.9995, Color{200, 20, 100, 255}};

	Image img(width, height);

	for(int h = 0; h < height; h++){
		for(int w = 0; w < width; w++){
			Vec3 dir = {w - 0.5 * width, h - 0.5 * height, 1};
			Color c = {0, 0, 0, 255};
			if(s.intersect(cam, dir)){
				c = s.color;
			}
			img.setPixel(w, h, c);
		}	
	}
	
	std::cout << "Write" << std::endl;
	write_to_disk(&img, "frame.png");
	
	return 0;
}