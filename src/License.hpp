#ifndef LICENSE
#define LICENSE

#include <string>
#include <vector>
// clang-format off

//namespace licence {

struct Resource {
  std::string license;
  std::string description;
  std::string link;
};

struct Author {
  std::string name;
  std::vector<Resource> resources;
};

std::vector<Author> authors  = 
{
  {
    "Arthur Sonzogni",
    {
      {
        "(CC BY-NC-ND 4.0)",
        "Everything in this game (software, graphics, sounds),\nunless otherwise stated below",
        "https://arthursonzogni.com"
      },
      {
        "(The MIT license)",
        "The Simple Multimedia Kit (SMK)",
        "https://github.com/ArthurSonzogni/smk"
      }
    }
  },
  {
    "JustInvoke",
    {
      {
        "(CC BY 3.0)",
        "Sound: skin/press_enter.wav",
        "https://freesound.org/people/JustInvoke/sounds/446142/"
      },
      {
        "(CC BY 3.0)",
        "Sound: skin/success.wav",
        "https://freesound.org/people/JustInvoke/sounds/446111/"
      }
    }
  }, 
  {
    "victorium183",
    {
      {
        "(CC0 1.0)",
        "Sound: skin/menu_change.wav",
        "https://freesound.org/people/victorium183/sounds/476816/"
      },
    }
  },
  {
    "Scrampunk",
    {
      {
        "(CC BY 3.0)",
        "Sound: skin/menu_select.wav",
        "https://freesound.org/people/Scrampunk/sounds/345297/"
      },
    }
  },
  {
    "soneproject",
    {
      {
        "(CC BY 3.0)",
        "Sound: skin/intro.wav",
        "https://freesound.org/people/soneproject/sounds/244356/"
      },
    }
  },
  {
    "SilverIllusionist",
    {
      {
        "(CC BY 3.0)",
        "Sound: skin/get_key.wav",
        "https://freesound.org/people/SilverIllusionist/sounds/411179/"
      }
    }
  },
  {
    "suntemple",
    {
      {
        "(CC0 1.0)",
        "Sound: skin/lose.wav",
        "https://freesound.org/people/suntemple/sounds/253174/"
      }
    }
  }
};

//} // namespace license

// clang-format on

#endif /* end of include guard: LICENSE */
