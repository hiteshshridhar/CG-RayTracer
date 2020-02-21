#include "float.h"
#include "vec3.h"
#include"sphere.h"
#include"hittable.h"
#include"hittableList.h"
#include "windows.h"
#include"ray.h"
#include <iostream>
#include <fstream>
#include "camera.h"
#include "random.h"
#include"lambertian.h"
//#include "metal.h"


vec3 lower_left_corner(-2.0, -1.0, -1.0); // plane starting corner
vec3 horizontal(4.0, 0.0, 0.0); //horizontal area to color
vec3 vertical(0.0, 2.0, 0.0); //vertical area to cover
vec3 origin(0.0, 0.0, 0.0); //origin of the camera
camera cam;




    // code shows where the ray hits the sphere and which color gets produced//

vec3 color(const ray& r, hittable *world, int depth) {
    hit_count rec;
  
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
            ray scattered;
            vec3 attenuation;
            if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation * color(scattered, world, depth + 1);
            }
            else {
                return vec3(0.5, 0.3, 0);
            }
        
       
        //vec3 target_pixel = rec.p + rec.normal + random_in_unit_sphere();
        //std::cout << "value of rec.p is: " << rec.p << std::endl;
        //std::cout << "value of rec.normal is: " << rec.normal << std::endl;
        //std::cout << "value of random in unit sphere  is: " << random_in_unit_sphere() << std::endl;
        //std::cout << "value of target is: " << target_pixel<<std::endl;
        //return 0.2* color(ray(rec.p, target-rec.p), world);
        //return 0.5 * color(ray(rec.p, target_pixel - rec.p), world); //return the color of pixel in matt
        //std::cout << "value of color and world is: " << x << world << std::endl;
       // return x;
        
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        //std::cout << "value of unit_direction is: " <<unit_direction << std::endl;
        float t = 0.5 * (unit_direction.y() + 1.0);
        //std::cout << "value of t is: " << t << std::endl;
        vec3 else_return =  (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 1.0, 1.0);//return the vertical background color value 
        //std::cout << "value of else_return is: " << else_return << std::endl;
       return else_return;
    }
}

int main() {

    int nx = 500;
    int ny = 300;
    int ns = 100; // number of samples
    // way to create the output file to see the contents of the image.
    
    std::ofstream output_file;
    CreateDirectoryA("output", NULL);
    output_file.open("output/rayTracer.ppm");
    output_file << "P3\n" << nx << " " << ny << "\n255\n";

    //-----end of the code snippet------//
    
    // creation of sphere and where the ray hits the sphere
    hittable *list[5];
    list[0] = new sphere (vec3(0, 0, -1), 0.5, new lambertian(vec3(1.0, 0.0, 0.0))); // sphere's x,y,z axis and radius 
    list[1] = new sphere (vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 1.0)));
    list[2] = new sphere(vec3(-1, 0.0, -1),0.5, new metal(vec3(0.8, 0.8, 0.8),0.5));
    list[3] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.5, 0.3), 0.5));
    list[4] = new sphere(vec3(0, 0.5, -1), 0.3, new dielectric(1.4));
    hittable *world = new hittableList(list, 5);      
    
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) 
        { 
            //loop for the printing of the color after defining where the color


            vec3 col(0, 0, 0);

            //runs unless the no of samples are not at single pixel antialiased and colors that single pixel
            
            for (int s = 0; s < ns; s++) { //for no. of samples through one pixel for antialiasing
                float u = float(i + random_double()) / float(nx);
                
                float v = float(j + random_double()) / float(ny);
                
                
                ray r = cam.get_ray(u, v);
                
                col += color(r, world,0); //to check and color the pixel if the ray hits the sphere
                
            }
            col /= float(ns);
            //std::cout << "value of col/ns is: " << col << std::endl;
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            //std::cout << "value of col_square_root is: " << col << std::endl;
            int ir = int(255.99 * col[0]);
            //std::cout << "value of ir is: " << ir << std::endl;
            int ig = int(255.99 * col[1]);
            //std::cout << "value of ig is: " << ig << std::endl;
            int ib = int(255.99 * col[2]);
            //std::cout << "value of ib is: " << ib << std::endl;


            output_file << ir << " " << ig << " " << ib << "\n";
        }
    }output_file.close();
}