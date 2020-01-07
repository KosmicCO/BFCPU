+                   sets first spot to 1 for the keyboard address
^.                  sends command to the keyboard to start sending keys
>^<.                sends command to the terminal to start receiving characters
[
    ^>,             polls the keyboard for characters
    [               if not the null character
        >^<.        write character to the terminal
        *           reset cells
    ]
    <               be on the keyboard address
]                   always loop (since keyboard address is 1)
