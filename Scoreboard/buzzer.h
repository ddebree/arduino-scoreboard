#ifndef D_BUZZER_H_
#define D_BUZZER_H_

class Buzzer {
  public:
    void init();

    bool isBuzzerOn();

    void buzzerShort();
    void buzzerShortShort();
    void buzzerLong();
  private:
    bool _buzzerOn;
    unsigned long _buzzerOffTime1;
    unsigned long _buzzerOnTime2;
    unsigned long _buzzerOffTime2;
};

#endif
