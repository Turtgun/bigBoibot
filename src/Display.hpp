#pragma once

#include "../include/display/lvgl.h"

#include "display/lv_misc/lv_area.h"

namespace Display {
    enum disp_obj_align {
        DISP_LEFT,
        DISP_CENTER,
        DISP_RIGHT
    };

    static inline lv_obj_t* lv_img_disp(const lv_img_dsc_t* cArr, lv_align_t align = LV_ALIGN_IN_TOP_LEFT, short x = 0, short y = 0)
    {
        lv_obj_t* img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, cArr);
        lv_obj_align(img, NULL, align, x, y);

        return img;
    }

    static inline lv_obj_t* createLabel(lv_obj_t* parent, lv_coord_t y, disp_obj_align align, lv_coord_t width, lv_coord_t height, const char* title)
    {
        lv_obj_t* label =  lv_label_create(parent, NULL);
        lv_obj_set_pos(label, (align+1)*50, y);
        lv_obj_set_size(label, width, height);
        lv_label_set_text(label, title);
        lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);

        return label;
    }

    static inline lv_obj_t* createBtn(lv_obj_t* parent, lv_coord_t y, disp_obj_align align, lv_coord_t width, lv_coord_t height, const char* title, 
                    lv_color_t rel, lv_color_t tglRel, lv_style_t* copy = &lv_style_plain, lv_color_t textColor = LV_COLOR_WHITE)
    {
        lv_obj_t* btn = lv_btn_create(parent, NULL);
        lv_obj_set_pos(btn, (align+1)*50, y);
        lv_obj_set_size(btn, width, height);

        lv_obj_t* label = lv_label_create(btn, NULL);
        lv_label_set_text(label, title);
        lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

        lv_style_t* btnStyle = new lv_style_t[4];

        for(unsigned char i = 0; i < 4; i++) lv_style_copy(&btnStyle[i], copy);

        lv_color_t pr = LV_COLOR_MAKE((uint8_t)(rel.red+10), (uint8_t)(rel.green+10), (uint8_t)(rel.blue+10));
        lv_color_t tglPr = LV_COLOR_MAKE((uint8_t)(tglRel.red+10), (uint8_t)(tglRel.green+10), (uint8_t)(tglRel.blue+10));

        btnStyle[0].body.main_color = rel;
        btnStyle[0].body.grad_color = rel;
        btnStyle[0].text.color = textColor;

        btnStyle[1].body.main_color = pr;
        btnStyle[1].body.grad_color = pr;
        btnStyle[1].text.color = textColor;

        btnStyle[2].body.main_color = tglRel;
        btnStyle[2].body.grad_color = tglRel;
        btnStyle[2].text.color = textColor;

        btnStyle[3].body.main_color = tglPr;
        btnStyle[3].body.grad_color = tglPr;
        btnStyle[3].text.color = textColor;

        lv_btn_set_style(btn, LV_BTN_STYLE_REL, &btnStyle[0]);
        lv_btn_set_style(btn, LV_BTN_STYLE_PR, &btnStyle[1]);
        lv_btn_set_style(btn, LV_BTN_STYLE_TGL_REL, &btnStyle[2]);
        lv_btn_set_style(btn, LV_BTN_STYLE_TGL_PR, &btnStyle[3]);

        return btn;
    }
    
    static inline void btnSetToggled(lv_obj_t* btn, bool toggled)
    {
        if(toggled != (lv_btn_get_state(btn) >= 2)) lv_btn_toggle(btn);
    }
};
