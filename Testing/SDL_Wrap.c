#include "SDL_Wrap.h"

/* Set up a simple (WIDTH, HEIGHT) window.
   Attempt to hide the renderer etc. from user. */
void Neill_SDL_Init(SDL_Simplewin *sw)
{

   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->finished = 0;

   sw->win= SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WWIDTH, WHEIGHT,
                          SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
   if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   SDL_SetRenderDrawBlendMode(sw->renderer,SDL_BLENDMODE_BLEND);

   /* Create texture for display */
   sw->display = SDL_CreateTexture(sw->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WWIDTH, WHEIGHT);
   if(sw->display == NULL){
      fprintf(stderr, "\nUnable to initialize SDL texture:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }
   SDL_SetRenderTarget(sw->renderer, sw->display);

   /* Clear screen, & set draw colour to black */
   SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, 255);
   SDL_RenderClear(sw->renderer);

}

/* Housekeeping to do with the render buffer & updating the screen */
void Neill_SDL_UpdateScreen(SDL_Simplewin *sw)
{
   SDL_SetRenderTarget(sw->renderer, NULL);
   SDL_RenderCopy(sw->renderer, sw->display, NULL, NULL);
   SDL_RenderPresent(sw->renderer);
   SDL_SetRenderTarget(sw->renderer, sw->display);
}

/* Gobble all events & ignore most */
void Neill_SDL_Events(SDL_Simplewin *sw)
{
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             sw->finished = 1;
       }
    }
}


/* Trivial wrapper to avoid complexities of renderer & alpha channels */
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, colour* rgb)
{
   SDL_SetRenderDrawColor(sw->renderer, rgb->r, rgb->g, rgb->b, SDL_ALPHA_OPAQUE);
}

void Neill_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT],
  unsigned char chr, int ox, int oy,colour* rgbf, colour* rgbb, heightmd md)
{
  unsigned x, y,yinit,yend,ydraw;
  if(md == sgl) {
    yinit = 0;
    yend = FNTHEIGHT;
  }
  else if (md == dbltop){
    yinit = 0;
    yend = FNTHEIGHT/2;
  }
  else {
    yinit = FNTHEIGHT/2;
    yend = FNTHEIGHT;
  }
  for(y = yinit; y < yend; y++){
     for(x = 0; x < FNTWIDTH; x++){
       if(md == sgl) {
         ydraw = y;
       }
       else {
         ydraw = 2*(y-yinit);
       }
        if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
           Neill_SDL_SetDrawColour(sw, rgbf);
           SDL_RenderDrawPoint(sw->renderer, x + ox, ydraw+oy);
           if(md) {
             SDL_RenderDrawPoint(sw->renderer, x + ox, ydraw+oy+1);
           }
        }
        else{
           Neill_SDL_SetDrawColour(sw, rgbb);
           SDL_RenderDrawPoint(sw->renderer, x + ox, ydraw+oy);
           if(md) {
             SDL_RenderDrawPoint(sw->renderer, x + ox, ydraw+oy+1);
           }
        }
     }
  }
}

void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname)
{
    FILE *fp = fopen(fname, "rb");
    size_t itms;
    if(!fp){
       fprintf(stderr, "Can't open Font file %s\n", fname);
       exit(1);
   }
   itms = fread(fontdata, sizeof(fntrow), FNTCHARS*FNTHEIGHT, fp);
   if(itms != FNTCHARS*FNTHEIGHT){
       fprintf(stderr, "Can't read all Font file %s (%d) \n", fname, (int)itms);
       exit(1);
   }
   fclose(fp);
}
