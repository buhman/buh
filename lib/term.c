#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#include <buh/buh.h>
#include <buh/term.h>

/**
 * @ingroup term
 * @{
 */

/**
 * Sets O_NONBLOCK on STDIN_FILENO
 * @returns nonzero on failure
 */
int
buh_term_nonblock(void)
{
  int ret;

  ret = fcntl(STDIN_FILENO, F_GETFL);
  if (ret < 0)
    herror("F_GETFL");

  ret = fcntl(STDIN_FILENO, F_SETFL, ret | O_NONBLOCK);
  if (ret < 0)
    herror("F_SETFL");

  return 0;
}

/**
 * Sets canonical mode and removes input echo
 * @returns nonzero on failure
 */
int
buh_term_setup(void)
{
  int ret;
  struct termios t;

  ret = tcgetattr(STDIN_FILENO, &t);
  if (ret < 0)
    herror("tcgetattr");

  t.c_lflag &= ~(ECHO | ECHONL | ICANON);

  ret = tcsetattr(STDIN_FILENO, TCSADRAIN, &t);
  if (ret < 0)
    herror("tcsetattr");

  return 0;
}
