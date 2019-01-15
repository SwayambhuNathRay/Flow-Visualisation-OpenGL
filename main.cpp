//
//  main.cpp
//  Opengl
//
//  Created by Swayambhu Nath Ray on 03/02/17.
//  Copyright © 2017 Swayambhu Nath Ray. All rights reserved.
//

#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>


using namespace std;
#define	NPN 600
#define NMESH  100
#define DM  ((float) (1.0/(NMESH-1.0)))
#define NPIX  600
#define SCALE 4.0

int click = 0;
float sa;
int iframe = 0;
float strength_of_source = 1.0;
int put_initial_source = 0;
int    alpha  = (0.12*255);
float  tmax   = NPIX/(SCALE*NPN);
float  dmax   = SCALE/NPIX;
int    Npat   = 32;
int type_of_flow=0;
int flow_visualization = 0;
int put_source_on_mouse_click = 0;
int type_of_source = 0;
float velocity[600][600];
int colour_of_flow = 0;
int change_colour_with_click = 0;
int current_source = 0;
float current_mouse_x = 0.5, current_mouse_y = 0.5;
float use_arrow = 0;
int keep_track = 0;
float x_init[400];
float y_init[400];
float x_fin[400];
float y_fin[400];

void initGL(void)
{
    glViewport(0, 0, (GLsizei) NPIX, (GLsizei) NPIX);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(-1.0, -1.0, 0.0);
    glScalef(2.0, 2.0, 1.0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);
}




void makePatterns(void)
{
    int lut[256];
    int phase[NPN][NPN];
    GLubyte pat[NPN][NPN][4];
    int i, j, k, t;

    for (i = 0; i < 256; i++)
        lut[i] = i < 127 ? 0 : 255;
    for (i = 0; i < NPN; i++)
        for (j = 0; j < NPN; j++)
            phase[i][j] = rand() % 256;
    if(flow_visualization == 0)
    {
    for (k = 0; k < Npat; k++) // The noise images are produced in a random manner
    {
        t = k*256/Npat;
        for (i = 0; i < NPN; i++)
        {
            if(colour_of_flow == 0)
            {
                for (j = 0; j < NPN; j++)
                {
                    pat[i][j][0] = 0;//lut[(t + phase[i][j]) % 255];
                    pat[i][j][1] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][2] = lut[(t + phase[i][j]) % 255]; //blue channel
                    pat[i][j][3] = alpha;
                }
            }
            else if(colour_of_flow == 1)
            {
                for (j = 0; j < NPN; j++)
                {
                    pat[i][j][0] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][1] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][2] = 0;//lut[(t + phase[i][j]) % 255]; //blue channel
                    pat[i][j][3] = alpha;
                }
            }
            else if(colour_of_flow == 2)
            {
                for (j = 0; j < NPN; j++)
                {
                    pat[i][j][0] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][1] = 0;//lut[(t + phase[i][j]) % 255];
                    pat[i][j][2] = lut[(t + phase[i][j]) % 255]; //blue channel
                    pat[i][j][3] = alpha;
                }
            }
            else if(colour_of_flow == 3)
            {
                for (j = 0; j < NPN; j++)
                {
                    pat[i][j][0] = 0;//lut[(t + phase[i][j]) % 255];
                    pat[i][j][1] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][2] = 0;//lut[(t + phase[i][j]) % 255]; //blue channel
                    pat[i][j][3] = alpha;
                }
            }
            else if(colour_of_flow == 4)
            {
                for (j = 0; j < NPN; j++)
                {
                    pat[i][j][0] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][1] = lut[(t + phase[i][j]) % 255];
                    pat[i][j][2] = lut[(t + phase[i][j]) % 255]; //blue channel
                    pat[i][j][3] = alpha;
                }
            }
        }
        glNewList(k + 1, GL_COMPILE);
        glTexImage2D(GL_TEXTURE_2D, 0, 4, NPN, NPN, 0, GL_RGBA, GL_UNSIGNED_BYTE, pat);
        glEndList();
    }
    }
    else
    {
        for (k = 0; k < Npat; k++) // The noise images are produced in a random manner
        {
            int colour[3600];
            int x_cood[3600];
            int y_cood[3600];
            int l = 0;
            for(int i = 1; i < NPN; i++)
            {
                for(int j = 1; j < NPN; j++)
                {
                    if((j%10) == 6 && (i%10) == 6)
                    {
                        float x = (float)i/600.0, y = (float)j/600.0;
                        float flow_x, flow_y;
                        float dx = x - current_mouse_x;
                        float dy = y - current_mouse_y;
                        float r = dx*dx + dy*dy;
                        if (r < 0.0001)
                            r = 0.0001;
                        if(type_of_flow==0)
                        {
                            flow_x = 0;
                            flow_y = 0;
                        }
                        else if(type_of_flow==1)
                        {
                            flow_x = 0.3;
                            flow_y = 0;
                        }
                        else if(type_of_flow == 2)
                        {
                            flow_x = 0;
                            flow_y = 0.3;
                        }
                        else if(type_of_flow == 3)
                        {
                            flow_x = 0.3;
                            flow_y = 0.3;
                        }
                        else if (type_of_flow == 4)
                        {
                            flow_x = 0.5 * sin(3 * cos(iframe * M_PI / 200.0) + 3*y);
                            flow_y = 0.5 * cos(3 * sin(iframe * M_PI / 200.0));
                            
                        }
                        else if(type_of_flow == 5)
                        {
                            flow_x = 0.5 * sin(3 * x * cos(iframe * M_PI / 200.0));
                            flow_y = 0.5 * cos(3 * x * sin(iframe * M_PI / 200.0));
                        }
                        
                        else if(type_of_flow == 6)
                        {
                            flow_x = 0.5 * sin(3 * x * x * cos(iframe * M_PI / 200.0) + 3 * x);
                            flow_y = 0.5 * cos(3 * y * y * sin(iframe * M_PI / 200.0) + 3 * y);
                        }
                        else if(type_of_flow == 7)
                        {
                            flow_x = 0.5 * sin(3 * x * x * cos(iframe * M_PI / 200.0) + 3 * y);
                            flow_y = 0.5 * cos(3 * x * x * sin(iframe * M_PI / 200.0) + 3 * y);
                        }
                        
                        float source = current_source;
                        float vx = source * sa * dx / r + flow_x;
                        float vy = source * sa * dy / r + flow_y;
                        r = vx*vx + vy*vy;
                        x_cood[l] = i;
                        y_cood[l] = j;
                        if(r < 0.1)
                            colour[l] = 1;
                        else if(r >= 0.1 && r < 0.6)
                            colour[l] = 2;
                        else if(r >= 0.6)
                            colour[l] = 3;
                        l++;
                    }
                }
            }
            for(int i = 0; i < 600; i++)
            {
                for(int j = 0; j < 600; j++)
                {
                    pat[i][j][0] = 255;
                    pat[i][j][1] = 255;
                    pat[i][j][2] = 255; //blue channel
                    pat[i][j][3] = alpha;
                }
            }
            for(int z = 0; z < 3600; z++)
            {
                if(colour[z] == 1)
                {
                    for(int p = x_cood[z] - 2; p <= x_cood[z]+2; p++)
                    {
                        for(int q = y_cood[z]-2; q<=y_cood[z]+2; q++)
                        {
                            pat[p][q][0] = 0;
                            pat[p][q][1] = 255;
                            pat[p][q][2] = 0; //blue channel
                            pat[p][q][3] = alpha;
                        }
                    }
                }
                else if(colour[z] == 2)
                {
                    for(int p = x_cood[z] - 2; p <= x_cood[z]+2; p++)
                    {
                        for(int q = y_cood[z]-2; q<=y_cood[z]+2; q++)
                        {
                            pat[p][q][0] = 0;
                            pat[p][q][1] = 0;
                            pat[p][q][2] = 255; //blue channel
                            pat[p][q][3] = alpha;
                        }
                    }
                }
                else if(colour[z] == 3)
                {
                    for(int p = x_cood[z] - 2; p <= x_cood[z]+2; p++)
                    {
                        for(int q = y_cood[z]-2; q<=y_cood[z]+2; q++)
                        {
                            pat[p][q][0] = 255;
                            pat[p][q][1] = 0;
                            pat[p][q][2] = 0; //blue channel
                            pat[p][q][3] = alpha;
                        }
                    }
                }

            }
            glNewList(k + 1, GL_COMPILE);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, NPN, NPN, 0, GL_RGBA, GL_UNSIGNED_BYTE, pat);
            glEndList();
        }
    }

}



void getDP_for_arrow(float x, float y, float *px, float *py, float x_mousepos, float y_mousepos, int check)
{
    float flow_x, flow_y;
    float dx = x - x_mousepos;
    float dy = y - y_mousepos;
    float r = dx*dx + dy*dy;
    if (r < 0.0001)
        r = 0.0001;
    if(type_of_flow==0)
    {
        flow_x = 0;
        flow_y = 0;
    }
    else if(type_of_flow==1)
    {
        flow_x = 0.3;
        flow_y = 0;
    }
    else if(type_of_flow == 2)
    {
        flow_x = 0;
        flow_y = 0.3;
    }
    else if(type_of_flow == 3)
    {
        flow_x = 0.3;
        flow_y = 0.3;
    }
    else if (type_of_flow == 4)
    {
        flow_x = 0.5 * sin(3 * cos(iframe * M_PI / 200.0) + 3*y);
        flow_y = 0.5 * cos(3 * sin(iframe * M_PI / 200.0));
        
    }
    else if(type_of_flow == 5)
    {
        flow_x = 0.5 * sin(3 * x * cos(iframe * M_PI / 200.0));
        flow_y = 0.5 * cos(3 * x * sin(iframe * M_PI / 200.0));
    }
    
    else if(type_of_flow == 6)
    {
        flow_x = 0.5 * sin(3 * x * x * cos(iframe * M_PI / 200.0) + 3 * x);
        flow_y = 0.5 * cos(3 * y * y * sin(iframe * M_PI / 200.0) + 3 * y);
    }
    else if(type_of_flow == 7)
    {
        flow_x = 0.5 * sin(3 * x * x * cos(iframe * M_PI / 200.0) + 3 * y);
        flow_y = 0.5 * cos(3 * x * x * sin(iframe * M_PI / 200.0) + 3 * y);
    }
    
    float source = check == -1 ? 1 : current_source;
    float vx = source * sa * dx / r + flow_x;
    float vy = source * sa * dy / r + flow_y;
    r = vx*vx + vy*vy;
    if (r > dmax*dmax)
    {
        r = sqrt(r);
        vx *= dmax / r;
        vy *= dmax / r;
    }
    
    
    *px = x + 6*vx;
    *py = y + 6*vy;
}

void getDP(float x, float y, float *px, float *py, float x_mousepos, float y_mousepos, int check)
{
    float flow_x, flow_y;
    float dx = x - x_mousepos;
    float dy = y - y_mousepos;
    float r = dx*dx + dy*dy;
    if (r < 0.0001)
        r = 0.0001;
    if(type_of_flow==0)
    {
        flow_x = 0;
        flow_y = 0;
    }
    else if(type_of_flow==1)
    {
        flow_x = 0.3;
        flow_y = 0;
    }
    else if(type_of_flow == 2)
    {
        flow_x = 0;
        flow_y = 0.3;
    }
    else if(type_of_flow == 3)
    {
        flow_x = 0.3;
        flow_y = 0.3;
    }
    else if (type_of_flow == 4)
    {
        flow_x = 0.5 * sin(3 * cos(iframe * M_PI / 200.0) + 3*y);
        flow_y = 0.5 * cos(3 * sin(iframe * M_PI / 200.0));

    }
    else if(type_of_flow == 5)
    {
        flow_x = 0.5 * sin(3 * x * cos(iframe * M_PI / 200.0));
        flow_y = 0.5 * cos(3 * x * sin(iframe * M_PI / 200.0));
    }
    
    else if(type_of_flow == 6)
    {
        flow_x = 0.5 * sin(3 * x * x * cos(iframe * M_PI / 200.0) + 3 * x);
        flow_y = 0.5 * cos(3 * y * y * sin(iframe * M_PI / 200.0) + 3 * y);
    }
    else if(type_of_flow == 7)
    {
        flow_x = 0.5 * sin(3 * x * x * cos(iframe * M_PI / 200.0) + 3 * y);
        flow_y = 0.5 * cos(3 * x * x * sin(iframe * M_PI / 200.0) + 3 * y);
    }

    float source = check == -1 ? 1 : current_source;
    float vx = source * sa * dx / r + flow_x;
    float vy = source * sa * dy / r + flow_y;
    r = vx*vx + vy*vy;
    if (r > dmax*dmax)
    {
        r = sqrt(r);
        vx *= dmax / r;
        vy *= dmax / r;
    }
    
    
    *px = x + vx;
    *py = y + vy;
}



float scaledim(float x, float dimension)
{
    float x_scaled = (x) / dimension;
    return x_scaled;

}

void mouse_click(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        if(change_colour_with_click==1)
        {
            colour_of_flow = (colour_of_flow+1)%5;
            makePatterns();
        }
        if(put_source_on_mouse_click == 1)
        {
        click = 1;
        if (button == GLUT_LEFT_BUTTON)
            current_source = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            current_source = -1;

        current_mouse_x = scaledim(x, glutGet(GLUT_INIT_WINDOW_WIDTH));
        current_mouse_y = 1 - scaledim(y, glutGet(GLUT_INIT_WINDOW_HEIGHT));
    }
    }
    glutPostRedisplay();
}


void key_func(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'a':
            change_colour_with_click = 1;
            break;
        case 'b':
            change_colour_with_click = 0;
            break;
        case 'c':
            put_source_on_mouse_click = 1;
            break;
        case 'd':
            put_source_on_mouse_click = 0;
            break;
        case 'e':
            type_of_flow = 0;
            break;
        case 'f':
            type_of_flow = 1;
            break;
        case 'g':
            type_of_flow = 2;
            break;
        case 'h':
            type_of_flow = 3;
            break;
        case 'i':
            type_of_flow = 4;
            break;
        case 'j':
            type_of_flow = 5;
            break;
        case 'k':
            type_of_flow = 6;
            break;
        case 'l':
            type_of_flow = 7;
            break;
        case 'm':
            type_of_source = 0;
            break;
        case 'n':
            type_of_source = 1;
            break;
        case '1':
            strength_of_source = 0.1;
            break;
        case '2':
            strength_of_source = 0.2;
            break;
        case '3':
            strength_of_source = 0.3;
            break;
        case '4':
            strength_of_source = 0.4;
            break;
        case '5':
            strength_of_source = 0.5;
            break;
        case '6':
            strength_of_source = 0.6;
            break;
        case '7':
            strength_of_source = 0.7;
            break;
        case '8':
            strength_of_source = 0.8;
            break;
        case '9':
            strength_of_source = 0.9;
            break;
        case 'o':
            put_initial_source = 1;
            current_mouse_x = 0.5;
            current_mouse_y = 0.5;
            break;
        case 'p':
            put_initial_source = 0;
            break;
        case 'q':
            click = 0;
            break;
        case 'r':
            flow_visualization = 1;
            makePatterns();
            break;
        case 's':
            flow_visualization = 0;
            makePatterns();
            break;
        case 't':
            use_arrow = 1;
            keep_track = 1;
            glutPostRedisplay();
            break;
        case 'u':
            use_arrow = 0;
            glutPostRedisplay();
            break;
    }
}



void special_key(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            current_mouse_y+=0.05;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            current_mouse_x+=0.05;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            current_mouse_y-=0.05;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            current_mouse_x-=0.05;
            glutPostRedisplay();
            break;
    }
}




void display(void)
{
    if(use_arrow == 0)
    {
    int   i, j;
    float x1, x2, y, px, py;
    if(type_of_source==0)
    {
        if(put_initial_source == 1)
        {
            sa = (put_initial_source)* strength_of_source * log(iframe*2.0*M_PI / 200.0);
        }
        else
        {
            sa = (click)* strength_of_source * log(iframe*2.0*M_PI / 200.0);
        }
    }
    else if(type_of_source == 1)
    {
        if(put_initial_source == 1)
        {
            sa = (put_initial_source)*strength_of_source*cos(iframe*2.0*M_PI/200.0);
        }
        else
        {
            sa = (click)*strength_of_source*cos(iframe*2.0*M_PI/200.0);
        }

    }
    for (i = 0; i < NMESH - 1; i++)
    {
        x1 = DM * i; x2 = x1 + DM;
        glBegin(GL_QUAD_STRIP);
        for (j = 0; j < NMESH; j++) {
            y = DM*j;
            glTexCoord2f(x1, y);
            if (click == 0)
            {
                getDP(x1, y, &px, &py, current_mouse_x, current_mouse_y, -1);
            }
            else
            {
                getDP(x1, y, &px, &py, current_mouse_x, current_mouse_y, 0);
            }
            glVertex2f(px,py);

            glTexCoord2f(x2, y);

            if (click == 0)
            {
                getDP(x2, y, &px, &py, current_mouse_x, current_mouse_y, -1);
            }
            else
            {
                getDP(x2, y, &px, &py, current_mouse_x, current_mouse_y, 0);
            }
            glVertex2f(px,py);
        }
        glEnd();
    }

    iframe = iframe + 1;
    glEnable(GL_BLEND);
    glCallList(iframe % Npat + 1);
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.0, 0.0);  glVertex2f(0.0, 0.0);
    glTexCoord2f(0.0, tmax); glVertex2f(0.0, 1.0);
    glTexCoord2f(tmax, 0.0);  glVertex2f(1.0, 0.0);
    glTexCoord2f(tmax, tmax); glVertex2f(1.0, 1.0);
    glEnd();
    glDisable(GL_BLEND);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     0, 0, NPIX, NPIX, 0);
    glutSwapBuffers();
    }
    else
    {
        glutSwapBuffers();
        glClearColor(1,1,1,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 0.0, 1.0);
        float px, py;
        if(keep_track == 1)
        {
        for(int i = 0; i < 400; i++)
        {
            float r = ((double) rand() / (RAND_MAX));
            x_init[i] = r;
            float s = ((double) rand() / (RAND_MAX));
            y_init[i] = s;
            getDP_for_arrow(r, s, &px, &py, current_mouse_x, current_mouse_y, 0);
            x_fin[i] = px;
            float slope = (float)(py-s)/(float)(px-r);
            y_fin[i] = s + slope*(px - r);
        }
            keep_track = 0;
        }
        glPointSize(5);
        for(int i = 0; i < 400; i++)
        {
            glBegin(GL_POINTS);
            glVertex2f(x_fin[i],y_fin[i]);
            glEnd();
        }
        glLineWidth(1);
        for(int i = 0; i <  400; i++)
        {
            glBegin(GL_LINES);
            glVertex2f(x_init[i], y_init[i]);
            glVertex2f(x_fin[i], y_fin[i]);
            glEnd();
            glFlush();
        }

        
    }
}



int main()
{
    int argc = 1;
    for(int i = 0; i < 600; i++)
        for(int j = 0; j < 600; j++)
            velocity[i][j] = 0;
    char* argv[1] = {(char*) "Some"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutFullScreen();
    glutInitWindowSize(NPIX, NPIX);
    glutCreateWindow("Flow Visualization");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutSpecialFunc(special_key);
    glutKeyboardFunc(key_func);
    glutMouseFunc(mouse_click);
    initGL();
    makePatterns();
    glutMainLoop();
    return 0;
}


