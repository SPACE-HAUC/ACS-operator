#include "../include/acs_util.h"
#include "../include/spacehauc-i2c-dev.h"
#include "../include/acs_operator.h"
#include "../include/subscriber.h"
#include "../include/publisher.h"

using namespace spacehauc_i2c;

int main(int argc, char const *argv[]) {

  publisher<bool> power_request("ACS_POWER_REQUEST", std::stoi(argv[1]));
  subscriber<bool> power_reply("ACS_POWER_REPLY", std::stoi(argv[1]));

  subscriber<bool> spin_stab_sub("SPIN_STAB", std::stoi(argv[1]));
  subscriber<bool> sun_point_sub("SUN_POINT", std::stoi(argv[1]));
  subscriber<bool> dark_side_sub("DARK_SIDE", std::stoi(argv[1]));

  subscriber<ACS_DATA_BLOCK> acs_sub("ACS_DATA", std::stoi(argv[1]));

  ACS_DATA_BLOCK my_data;

  while (true) {
    while (!dark_side_sub.get_data()) { // LIGHT SIDE STUFF

      if (!spin_stab_sub.get_data()) { // NOT SPIN STABBED
        my_data = acs_sub.get_data();
        power_request.publish(true);
        if (power_reply.get_data()) {
         stabilize_spin(my_data);
       }
     }

      else { // MEANS YOU ARE SPIN STABILIZED
        if (!sun_point_sub.get_data()) {
          my_data = acs_sub.get_data();
          power_request.publish(true);
          if (power_reply.get_data()) {
           point_to_sun(my_data);
         }
       }
       else {
          power_request.publish(false);
         sleep(2);
        } // END OF SUN POINT

      } // END OF SPIN STAB LOOP

    } // END OF LIGHT SIDE LOOP

    power_request.publish(false);
    // SET TORQUERS TO 0

    while (dark_side_sub.get_data()) { // DARK SIDE STUFF
      //J chill and wait for light side
      sleep(5);
    }
  }


  return 0;
}

void stabilize_spin(ACS_DATA_BLOCK data) {
  std::cout << "DO THE THING" << std::endl;
}
void point_to_sun(ACS_DATA_BLOCK data) {
  std::cout << "DO THE THING" << std::endl;
}
