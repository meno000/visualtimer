 
/* Visual Timer 

Idea is for N (green) LEDs and a (red) LED, 
greens all start on, fade out one by one over the specified time
(10s), and then the red one goes on.

Essentially, we do the greens in a row, + to 1k resistor across 
channel, - to common ground, resistor to pin, see setup(), 
assigned in order, 0-5, 5 next to red LED. (red LED as above, 
connected to pin = RED.) 

Buttons: 1 near red LED (mode/timer start),
where button toggles between "set mode", red LED on and
timer set button works as below, and "timer mode" where
timer counts down starting at toggle. (i.e., double press resets
timer.)
one near initial green LED ("timer set"), moves timer value among
1 (no LED lit) , 2 (1st LED lit), 5 (2nd LED lit), 8 (3rd), 
15 (4th), 30 (5th), 60 (6th).

Note, version 1 will only stay in "timer set" mode after the 
timer finishes running (hypothetically, 
red LED will turn on, off, on and then stay on.


*/

int RED = 2;
int start_button_pin = 8;
int set_button_pin = 12;
int green_led_pins[] = {3, 5, 6, 9, 10, 11};

int start_button_state = 0;
int set_button_state = 0;

int set_button_pressed = 0;

int time_lvl_idx = 0;

const int NUM_LEDS = 6;

const int STEPS_PER_LED = 5;
int increment = 50;


int time_lvl[] = {1000,2000,5000,8000,15000,30000};
int interval = 500;

struct gled
{
  int pin;
  int brightness;
};

struct gled body[NUM_LEDS];

enum modes {
  vtimer_set,
  vtimer
};

int mode = vtimer_set;

void setup()
{
    
  pinMode(RED, OUTPUT);
  pinMode(start_button_pin, INPUT);
  pinMode(set_button_pin, INPUT);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    body[i].pin = green_led_pins[i];
    pinMode(body[i].pin, OUTPUT);

  }


  mode = vtimer_set;
}

void loop()
{
  if (mode == vtimer_set)
  {
    digitalWrite(RED, HIGH);
    start_button_state = digitalRead(start_button_pin);
    
    // using set_button
    set_button_state = digitalRead(set_button_pin);
   
    for (int i = 0; i < NUM_LEDS; i++)
    {
          body[i].brightness = (i == time_lvl_idx) ? 255 : 0;
          analogWrite(body[i].pin, body[i].brightness);

    }
    
    interval = time_lvl[timer_val_idx]/(NUM_LEDS * STEPS_PER_LED);
    increment = 255/STEPS_PER_LED;

 
    
    if (set_button_state == HIGH && set_button_pressed == 0)
    {
      set_button_pressed = 1;
      
    }
    
    
    if (set_button_pressed == 1 && set_button_state == LOW)
    {
      timer_val_idx++;
      
      if (timer_val_idx == NUM_LEDS)
      {
        timer_val_idx = 0;
      }
      

      set_button_pressed = 0;
    }

    
    if (start_button_state == HIGH)
    {
      mode = vtimer;
    }
    
  } else if (mode == vtimer)
  {
  
    digitalWrite(RED, LOW);
  
  for (int i = 0; i < NUM_LEDS; i++)
  {
    body[i].brightness = 255;
    analogWrite(body[i].pin, body[i].brightness);
  }

  for (int i = 0; i < NUM_LEDS; i++)
  {
    while (body[i].brightness > 0)
    {
      analogWrite(body[i].pin, body[i].brightness);
      body[i].brightness -= increment;
      delay(interval);
      
 //     start_button_state = digitalRead(start_button_pin);
 //     if (start_button_state == HIGH)
 //     {
 //       mode = vtimer_set;
 //     }
    }
    body[i].brightness = 0;
    analogWrite(body[i].pin, body[i].brightness);
  }
//  digitalWrite(RED, HIGH);
//  delay(1000);
//  start_button_state = digitalRead(start_button_pin);
//  if (start_button_state == HIGH)
//  {
    mode = vtimer_set;
//  }
//  digitalWrite(RED, LOW);
  }
}
