#include "undo_stack.h"

#include <iostream>

using namespace dak::utility;
using namespace std;

struct sine
{
   // Primary data.
   double amplitude = 1.;
   double frequency = 1.;
   int cycles = 1;
   static constexpr int samples_per_cycle = 50;

   // Secondary data.
   vector<double> wave;

   sine(double amp, double freq, int cy)
   : amplitude(amp), frequency(freq), cycles(cy)
   {
      fill_data();
   }

   void fill_data()
   {
      wave.clear();
      wave.reserve(cycles * samples_per_cycle);
      for (int cycle = 0; cycle < cycles; ++cycle)
         for (int sample = 0; sample < samples_per_cycle; ++sample)
            wave.emplace_back(std::sin(sample * frequency / samples_per_cycle) * amplitude);
   }

   void clear_data()
   {
      wave.clear();
   }
};

int main()
{
   // The undo stack of the app.
   undo_stack undos;

   // The application data.
   sine app_sine(30., 20., 5);

   // A function to commit data.
   auto commit_data = [&undos, &app_sine]()
   {
      undos.commit(
         // The array of data to commit. Only one item in this example.
         {
            undo_data(
            {
               // The data to save. Will be assigned to the std::any.
               app_sine,
               // The deaden lambda. Clears the secondary data.
               [](std::any& data){ std::any_cast<sine&>(data).clear_data(); },
               // The awaken lambda. Recreates the secondary data and assigns it to the application sine.
               [&app_sine](const std::any& data){ app_sine = std::any_cast<const sine&>(data); app_sine.fill_data(); },
            })
         }
      );
   };

   // Commit the initial data as the earliest baseline we can come back to it when we undo later.
   commit_data();

   // User modify the app data. Commit it.
   app_sine = sine(12.5, 10., 7);
   commit_data();

   // More modifications to the app data. Commit it.
   app_sine = sine(47., 22., 3);
   commit_data();

   // A function to show the current app data.
   auto show_app_data = [&app_sine]()
   {
      cout << "  amplitude: " << app_sine.amplitude << endl
           << "  frequency: " << app_sine.frequency << endl
           << "  cycles:    " << app_sine.cycles << endl
           << "-------------" << endl;
   };

   // Now we will undo and redo and show the app data being updated.
   cout << "Current data:" << endl;
   show_app_data();

   cout << "After one undo:" << endl;
   undos.undo();
   show_app_data();

   cout << "After two undos:" << endl;
   undos.undo();
   show_app_data();

   cout << "After one redo:" << endl;
   undos.redo();
   show_app_data();

   cout << "After two undos:" << endl;
   undos.redo();
   show_app_data();
}
