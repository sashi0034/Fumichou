// This is AngelScript for debug.

bool s_interrupted = false;

void setup()
{
    // Remove below if debugging.
    return;

    // Set events on NMI
    watch_interrupt(function () {
        if (s_interrupted) return;
        s_interrupted = true;
        breakpoint();
    });
}
