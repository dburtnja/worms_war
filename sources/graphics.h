#pragma once

#include <gtkmm.h>
#include "board.h"

class WormArea : public Gtk::DrawingArea {
 private:
  Board board_;
  bool debug_;

 protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  void drawText(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y, int id);

 public:
  WormArea(int width, int height);
  virtual ~WormArea() { }
  void forceRedraw();
  void setDebug(bool debug) { debug_ = debug; }
};

class Graphics : public Gtk::Window {
 private:
  WormArea drawingArea_;
 public:
  // Seting up and running the application.
  static int createAndRun(int argc, char** argv,
      int width, int height, bool debug = false);

  Graphics(int width, int height, bool debug = false);
  virtual ~Graphics() { }
};
