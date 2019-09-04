#include "graphics.h"

#include <iostream>

using Gtk::DrawingArea;

WormArea::WormArea(int width, int height)
    : board_(width, height), debug_{false} {
  for (int i = 0; i < 50; ++i) {
    board_.addWorm(Lazy, rand() % board_.getWidth(),
                   rand() % board_.getHeight());
  }
  for (int i = 0; i < 20; ++i) {
    board_.addWorm(Hunter, rand() % board_.getWidth(),
                   rand() % board_.getHeight());
  }
  board_.addWorm(User, board_.getWidth() / 2, board_.getHeight() / 2);
  add_events(Gdk::ALL_EVENTS_MASK);
}

bool WormArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  const int bWidth = board_.getWidth();
  const int bHeight = board_.getHeight();

  const int fieldWidth = width / bWidth;
  const int fieldHeight = height / bHeight;

  cr->save();
  cr->set_source_rgb(0.0,0.0,0.0);
  for (int y = 0; y < bHeight; ++y) {
    for (int x = 0; x < bWidth; ++x) {
      if (!board_.isWormAt(x,y)) continue;

      int id = board_.at(x, y);
      if (board_.getWormType(id) == Hunter)
        cr->set_source_rgb(1.0,0.0,0.0);
      if (board_.getWormType(id) == User)
        cr->set_source_rgb(0.0,1.0,0.0);

        // Draw circle
      double radious = (std::min(fieldWidth,fieldHeight) / 2.5) * board_.getWormSize(id);
      cr->arc(x*fieldWidth + fieldWidth/2, y*fieldHeight + fieldHeight/2,
              radious, 0.0, 2.0 * M_PI);
      if (debug_) {
        cr->stroke();
        // Draw id inside circle
        this->drawText(cr, x*fieldWidth + fieldWidth/2,
            y*fieldHeight + fieldHeight/2, id);
      } else {
        // Fill the circle
        cr->fill_preserve();
      }
      cr->stroke();
      cr->set_source_rgb(0.0,0.0,0.0);
    }
  }
  cr->restore();
  cr->stroke();

  this->forceRedraw();

  return true;
}

void WormArea::drawText(const Cairo::RefPtr<Cairo::Context>& cr,
    int x, int y, int id) {
  Pango::FontDescription font;
  font.set_family("Monospace");
  font.set_weight(Pango::WEIGHT_BOLD);
  auto layout = create_pango_layout(std::to_string(id));
  layout->set_font_description(font);

  int textWidth;
  int textHeight;

  //get the text dimensions (it updates the variables -- by reference)
  layout->get_pixel_size(textWidth, textHeight);

  // Position the text in (x,y)
  cr->move_to(x - textWidth/2,y - textHeight/2);

  layout->show_in_cairo_context(cr);
}

void WormArea::forceRedraw()
{
  auto win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                     get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
}

bool WormArea::on_motion_notify_event(GdkEventMotion *eventMotion) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    const int bWidth = board_.getWidth();
    const int bHeight = board_.getHeight();

    double x = (eventMotion->x * bWidth) / width;
    double y = (eventMotion->y * bHeight) / height;
    board_.setMousePosition(x, y);
    return true;
}

// Static function to set up and run the application.
int Graphics::createAndRun(int argc, char** argv,
    int width, int height, bool debug) {
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.worms");
  Graphics window(width, height, debug);
  return app->run(window);
}

Graphics::Graphics(int width, int height, bool debug)
    : drawingArea_(width, height) {
  set_title("Worms");
  set_default_size(1800,900);
  drawingArea_.setDebug(debug);
  add(drawingArea_);
  show_all_children();
  srand(time(nullptr));
}

