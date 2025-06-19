# NEEDED

- Seriell input should be echoed back. The input is max 10 char long and appears
  max every 30 seconds.
- main function structure:
  ```c
  while (1) {
    sense();
    process();
    actuate();
  }
  ```
- no magic numbers. the roboter should be configurable in the main function
  before the infinit-loop.
