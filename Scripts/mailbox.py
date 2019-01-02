def mailbox_data():
    return [
        {"section": "UI Mobile", "endpoints": [
            {"name": "uiLoaded", "args": [], "emit": ["qml"], "signal": ["cpp"] },
            {"name": "done", "args": ["JStorage storage"], "emit": ["qml"], "signal": ["cpp"] },
        ]},
    ]
