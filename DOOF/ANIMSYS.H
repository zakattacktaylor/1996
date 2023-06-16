long frames=0;
long frame_time;
int pg_now;
        
void init_anim(void);
void flip_page(void);
void kill_anim(void);

void init_anim() {
        int i;
        
        set_modex(vga_mode);
        pg_now=0;
        set_display_page(0);
        set_active_page(2);
        for (i=0;i<100;i++)
                fill_block(0,i,319,i,236-i);
        for (i=100;i<240;i++)
                fill_block(0,i,319,i,((i/2-50)+10));
        
        set_active_page(1);
        clear_vga_screen(0);
        frame_time=time(NULL);
        VIDEO_ON=1;
}

void flip_page() {
        if (pg_now) {
                set_display_page(0);
                copy_page(2,1);
                set_active_page(1);
                //clear_vga_screen(0);
                pg_now=0;
        }
        else {
                set_display_page(1);
                copy_page(2,0);
                set_active_page(0);
                //clear_vga_screen(0);
                pg_now=1;
        }
        frames++; 
}

void kill_anim() {
        _asm mov ax,03h
        _asm int 10h
        VIDEO_ON=0;
        printf("%d frames per second\n",frames/(time(NULL)-frame_time));
}
