import omnific

class omnific_script:

    def __init__(self):
        pass

    def on_input(self):
        log_inputs = ["q", "p", "button_a"]
        shutdown_inputs = ["z"]
        restart_inputs = ["x"]
        playerID = 0
        inputs = omnific.get_input()
        logger = omnific.get_logger()
    
        if inputs.is_on_release(log_inputs, playerID):
            logger.write("one of " + str(log_inputs) + " was pressed.")
        if inputs.is_on_release("z"):
            logger.write("shutting down on input request...\n")
            inputs.force_shutdown()
        if inputs.is_on_release("x"):
            logger.write("restarting on input request...")
            inputs.force_restart()
        if inputs.is_on_release("button_b"):
            haptic_playerID = 0
            haptic_strength = 1.0
            haptic_duration_ms = 1000
        
            scene = omnific.get_scene()    
            scene.get_haptic_signal_buffer().publish(
                haptic_playerID,
                haptic_strength,
                haptic_duration_ms)
            logger.write("Sent haptic signal; playerID: " +
                                   str(haptic_playerID) + ", strength: " +
                                   str(haptic_strength) + ", duration: " +
                                   str(haptic_duration_ms) + "ms")
