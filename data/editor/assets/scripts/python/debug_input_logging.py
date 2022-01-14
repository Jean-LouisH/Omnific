import esi

def on_input():
    log_inputs = ["q", "p", "button_a"]
    shutdown_inputs = ["z"]
    restart_inputs = ["x"]
    playerID = 0
    input_api = esi.get_input_api()
    log_api = esi.get_log_api()
    
    if input_api.is_on_release(log_inputs, playerID):
        log_api.write("one of " + str(log_inputs) + " was pressed.")
    if input_api.is_on_release("z"):
        log_api.write("shutting down on input request...\n")
        input_api.force_shutdown()
    if input_api.is_on_release("x"):
        log_api.write("restarting on input request...")
        input_api.force_restart()
    if input_api.is_on_release("button_b"):
        haptic_playerID = 0
        haptic_strength = 1.0
        haptic_duration_ms = 1000
        
        scene = esi.get_scene_api().get_scene()    
        scene.get_haptic_signal_buffer().publish(
            haptic_playerID,
            haptic_strength,
            haptic_duration_ms)
        log_api.write("Sent haptic signal; playerID: " +
                               str(haptic_playerID) + ", strength: " +
                               str(haptic_strength) + ", duration: " +
                               str(haptic_duration_ms) + "ms")
