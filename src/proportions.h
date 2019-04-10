#ifndef PROPORTIONS_H
#define PROPORTIONS_H

#include "ofMain.h"

// Those are some proportions for use with the warping, and especially usefull for the surface number display, when 'test screen' are displayed.

struct proportions {

    float x;
    float y;
    float width;
    float height;
    bool bAdapt_to_warper = false;

    proportions(float &x, float &y, float &width, float &height, ofRectangle *bB_1d, ofRectangle *bB_2d, float *size_factor)
        : x(x), y(y), width(width), height(height), bB_1d(bB_1d), bB_2d(bB_2d), size_factor(size_factor) {adapt_to_media();}

    void adapt_to_media(){
        bAdapt_to_warper = false;
        float font_quot_width  = width / bB_1d->width;
        float font_quot_height = height / bB_1d->height;
        if (font_quot_width < font_quot_height){
            font_size_factor_1dig = font_quot_width * (*size_factor);
        }else{
            font_size_factor_1dig = font_quot_height * (*size_factor);
        }
        font_quot_width  = width / bB_2d->width;
        font_quot_height = height / bB_2d->height;
        if (font_quot_width < font_quot_height){
            font_size_factor_2dig = font_quot_width * (*size_factor);
        }else{
            font_size_factor_2dig = font_quot_height * (*size_factor);
        }
        translate_1d = glm::vec3(x + (width - bB_1d->width * font_size_factor_1dig)/2.0f, y + (height + bB_1d->height * font_size_factor_1dig)/2.0f, 0.0f);
        translate_2d = glm::vec3(x + (width - bB_2d->width * font_size_factor_2dig)/2.0f, y + (height + bB_2d->height * font_size_factor_2dig)/2.0f, 0.0f);
    }

    void adapt_to_warper(ofRectangle warBasRec){
        bAdapt_to_warper = true;
        float font_quot_width  = warBasRec.width / bB_1d->width;
        float font_quot_height = warBasRec.height / bB_1d->height;
        if (font_quot_width < font_quot_height){
            font_size_factor_1dig = font_quot_width * (*size_factor);
        }else{
            font_size_factor_1dig = font_quot_height * (*size_factor);
        }
        font_quot_width  = warBasRec.width / bB_2d->width;
        font_quot_height = warBasRec.height / bB_2d->height;
        if (font_quot_width < font_quot_height){
            font_size_factor_2dig = font_quot_width * (*size_factor);
        }else{
            font_size_factor_2dig = font_quot_height * (*size_factor);
        }
        translate_1d = glm::vec3(warBasRec.x + (warBasRec.width - bB_1d->width * font_size_factor_1dig)/2.0f, warBasRec.y + (warBasRec.height + bB_1d->height * font_size_factor_1dig)/2.0f, 0.0f);
        translate_2d = glm::vec3(warBasRec.x + (warBasRec.width - bB_2d->width * font_size_factor_2dig)/2.0f, warBasRec.y + (warBasRec.height + bB_2d->height * font_size_factor_2dig)/2.0f, 0.0f);
    }

    glm::vec3 getTranslationVector(unsigned int &surface_nb){
        if(surface_nb < 10){
            return translate_1d;
        }else {
            return translate_2d;
        }
    }

    float getFontFactor(unsigned int &surface_nb){
        if (surface_nb < 10){
            return font_size_factor_1dig;
        }else{
            return font_size_factor_2dig;
        }
    }

private:
    float font_size_factor_1dig, font_size_factor_2dig;
    glm::vec3 translate_1d, translate_2d;
    ofRectangle *bB_1d, *bB_2d;
    float *size_factor;
};

#endif // PROPORTIONS_H
