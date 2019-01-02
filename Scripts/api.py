def api_data():
    return {
      "structs": [
        {
          "name": "JErr",
          "fields": [
            "JBool successful",
            "JString reason"
          ]
        },
        {
          "name": "JEmpty",
          "fields": []
        },
        { "name": "JGoal",
          "fields": [
            "JString goal",
            "JInt count",
          ]
        },
        { "name": "JStorage",
          "fields": [
            "JArray<JGoal> goals",
          ]
        },
      ],
      "sections": [
      ]
    }
