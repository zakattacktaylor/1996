void put_hud(void)
{
        char s1[10];
        char s2[10];
        char s3[10];
        char s4[10];
        char s5[10];
        char s6[10];
        char s7[10];

        itoa(objects[0].xan,s1,10);
        itoa(objects[0].yan,s2,10);
        itoa(objects[0].zan,s3,10);
        itoa(camera.x,s4,10);
        itoa(camera.y,s5,10);
        itoa(camera.z,s6,10);
        itoa(curious,s7,10);

        print_str(s1,3,20,220,200,0);
        print_str(s2,3,60,220,202,0);
        print_str(s3,3,100,220,204,0);
        print_str(s4,6,140,220,206,0);
        print_str(s5,6,180,220,208,0);
        print_str(s6,6,220,220,210,0);
        print_str(s7,3,260,220,212,0);
}
