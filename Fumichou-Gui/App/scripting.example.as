// This is AngelScript for debug.

bool s_interrupted = false;

void setup()
{
    // Remove below when debugging
    return;

    // Set events on specific PC
    watch_pc(0x8000, function () {
        breakpoint();
    });

    // Set events on NMI
    watch_interrupt(function () {
        if (s_interrupted) return;
        s_interrupted = true;
        breakpoint();
    });
}
