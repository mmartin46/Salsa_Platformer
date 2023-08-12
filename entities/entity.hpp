#ifndef ENTITY_H
#define ENTITY_H

// An entity is a item, enemy, etc. that
// isn't a player and has a sprite.
class Entity
{
   private:
      int x, y, w, h;
   public:
      inline virtual int get_x() { return x; } const
      inline virtual void set_x(int val) { x = val; }
      inline virtual int get_y() { return y; } const
      inline virtual void set_y(int val) { y = val; }

      inline virtual int get_w() { return w; } const
      inline virtual void set_w(int val) { w = val; }
      inline virtual int get_h() { return h; } const
      inline virtual void set_h(int val) { h = val; }    
};


#endif