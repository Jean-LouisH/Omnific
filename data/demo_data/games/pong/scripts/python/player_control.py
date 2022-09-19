import omnia

class omnia_script:

    def __init__(self):
        pass

    def on_logic(self):
        transform = omnia.get_component("Transform")
        inp = omnia.get_input()
        if inp.is_pressed("w"):
            transform.translate_y(1.0)
        elif inp.is_pressed("s"):
            transform.translate_y(-1.0)
        else:
            transform.translate_y(inp.get_axis("left_axis_y"))
        pass
    
