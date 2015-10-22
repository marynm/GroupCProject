import audio
import unittest


class TestSequenceFunctions(unittest.TestCase):

    def test_play_song(self):
        # play a song that exsists in the directory
        self.assertEqual(audio.play_song('song1.mp3'),'Playing:song1.mp3')

    def test_song_DNE(self):
		#attempt to play a sond that does not exist
        self.assertEqual(audio.play_song('song2.mp3'),'Song "song2.mp3" does not exist.')

if __name__ == '__main__':
    unittest.main()
