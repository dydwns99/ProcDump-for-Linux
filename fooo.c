#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>


int main(int argc, char *argv[])
{
        Display *dpy;
        int screen;
        Window win;
        Window childwin;
        XEvent event;

        int x, y, width, height;
        Window root_win;
        Colormap colormap;
        XColor button_color;
        XColor lightgray_color, darkgray_color;
        XGCValues gcv_lightgray, gcv_darkgray;
        GC gc_lightgray, gc_darkgray;
        unsigned int border_width;
        unsigned int depth;

	/* Text declaration vars */
        XFontStruct *font;
        XTextItem ti[1];

        dpy = XOpenDisplay(NULL);

        if(dpy == NULL)
        {
                fprintf(stderr, "Cannot open display\n");
                exit(1);
        }

        screen = DefaultScreen(dpy);

        /*parent window*/
        win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
                200, 200, 500, 300,
                1, BlackPixel(dpy, screen), WhitePixel(dpy, screen));

        XSelectInput(dpy, win, ExposureMask | KeyPressMask);
        XMapWindow(dpy, win);


        /*child window*/

        colormap = DefaultColormap(dpy,screen);
        XParseColor(dpy, colormap, "rgb:cc/cc/cc", &button_color);
        XAllocColor(dpy, colormap, &button_color);

        XParseColor(dpy, colormap,"rgb:ee/ee/ee",&lightgray_color);
        /*XAllocColor(dpy, colormap, &lightgray_color.pixel);*/
        gcv_lightgray.foreground = lightgray_color.pixel;
        gcv_lightgray.background = button_color.pixel;
        gc_lightgray = XCreateGC(dpy,RootWindow(dpy,screen),GCForeground | GCBackground, &gcv_lightgray);

        XParseColor(dpy, colormap,"rgb:88/88/88",&darkgray_color);
        /*XAllocColor(dpy, colormap, &darkgray_color.pixel);*/
        gcv_darkgray.foreground = darkgray_color.pixel;
        gcv_darkgray.background = button_color.pixel;
        gc_darkgray = XCreateGC(dpy,RootWindow(dpy,screen),GCForeground | GCBackground, &gcv_darkgray);



        childwin = XCreateSimpleWindow(dpy,win,
        20, 20, 200, 100,
        1,BlackPixel(dpy, screen), button_color.pixel);

        XSelectInput(dpy, childwin, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask);
        XMapWindow(dpy, childwin);

        XGetGeometry(dpy,childwin,&root_win, &x, &y, &width, &height, &border_width, &depth);

        while(1)
        {
                XNextEvent(dpy, &event);
                if(event.xany.window == childwin)
                {
                        if(event.type == Expose)
                        {
				XDrawLine(dpy, childwin, gc_darkgray,0,0,width-1,0);
                                XDrawLine(dpy, childwin, gc_darkgray,0,0,0,height-1);
                                XDrawLine(dpy, childwin, gc_lightgray,width-1,0,width-1,height-1);
                                XDrawLine(dpy, childwin, gc_lightgray,0,height-1,width-1,height-1);

                                font = XLoadQueryFont(dpy, "9x15");
                                ti[0].chars = "Press me!";
                                ti[0].nchars =9;
                                ti[0].delta =0;
                                ti[0].font = font->fid;
                                XDrawText(dpy, childwin, DefaultGC(dpy, screen), 
					(width-XTextWidth(font, ti[0].chars, ti[0].nchars))/2,
					(height-(font->ascent+font->descent))/2+font->ascent,
					ti, 1);
                                XUnloadFont(dpy, font->fid);

                        }
                        if(event.type == ButtonPress)
                        {
                                if(event.xbutton.button == 1)
                                {
                                        XDrawLine(dpy, childwin, gc_darkgray,0,0,width-1,0);
                                        XDrawLine(dpy, childwin, gc_darkgray,0,0,0,height-1);
                                        XDrawLine(dpy, childwin, gc_lightgray,width-1,0,width-1,height-1);
                                        XDrawLine(dpy, childwin, gc_lightgray,0,height-1,width-1,height-1);
                                        printf("Button Pressed!\n");
                                }

                        }
                        if(event.type == ButtonRelease)
                        {
                                if(event.xbutton.button == 1)
                                {

                                        XDrawLine(dpy, childwin, gc_lightgray,0,0,width-1,0);
                                        XDrawLine(dpy, childwin, gc_lightgray,0,0,0,height-1);
                                        XDrawLine(dpy, childwin, gc_darkgray,width-1,0,width-1,height-1);
                                        XDrawLine(dpy, childwin, gc_darkgray,0,height-1,width-1,height-1);
                                        printf("Button Released!\n");
                                }

                        }
                }

        }
        return 0;
}

