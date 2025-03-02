#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <IOKit/graphics/IOGraphicsLib.h>
#include <ApplicationServices/ApplicationServices.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("expected argument\n");
    return -1;
  }

  double param = strtod(argv[1], 0);
  if (param < 0. || param > 1.) {
    printf("expected number between 0 and 1\n");
    return -1;
  }

  CFMutableDictionaryRef matchingDict = IOServiceNameMatching("AppleBacklightDisplay");
  io_service_t service = IOServiceGetMatchingService(kIOMainPortDefault, matchingDict);
  assert(service);

  CFMutableDictionaryRef dp = (CFMutableDictionaryRef)IORegistryEntryCreateCFProperty(service, CFSTR("IODisplayParameters"), kCFAllocatorDefault,0);
  assert(dp);
  CFMutableDictionaryRef a = (CFMutableDictionaryRef)CFDictionaryGetValue(dp, CFSTR("bklt"));
  assert(a);

  int max, min;
  CFNumberRef rmax = CFDictionaryGetValue(a, CFSTR("max"));
  assert(rmax);
  CFNumberRef rmin = CFDictionaryGetValue(a, CFSTR("min"));
  assert(rmin);
  CFNumberGetValue(rmax, kCFNumberSInt32Type, &max);
  CFNumberGetValue(rmin, kCFNumberSInt32Type, &min);

  int i = min + (max - min) * param;
  assert(i >= min && i <= max);

  printf("setting bklt to 0x%x (min 0x%x max 0x%x)\n", i, min, max);

  CFNumberRef n = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &i);
  CFDictionarySetValue(a, CFSTR("value"), n);
  IORegistryEntrySetCFProperty(service, CFSTR("IODisplayParameters"), dp);

  return 0;
}
