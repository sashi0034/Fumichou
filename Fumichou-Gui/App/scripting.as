// This is AngelScript for debug.
void reload(){
    // Set events on a specific PC
    watch_pc(0xA4FF, function(){
            // Branch on A register value.
            if (get_a() != 0x00){
                // Fire breakpoint.
                breakpoint();
            }
        }
    );
}
