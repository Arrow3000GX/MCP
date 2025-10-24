#!/usr/bin/env node

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
  CallToolRequestSchema,
  ListToolsRequestSchema,
} from '@modelcontextprotocol/sdk/types.js';

// Audiobook MCP Server
class AudiobookMCPServer {
  constructor() {
    this.server = new Server(
      {
        name: 'audiobook-mcp',
        version: '1.0.0',
      },
      {
        capabilities: {
          tools: {},
        },
      }
    );

    this.currentBook = null;
    this.currentChapter = 1;
    this.isPlaying = false;
    this.playbackSpeed = 1.0;
    this.currentPosition = 0; // in seconds
    this.bookmarks = new Map(); // book title -> array of bookmarks
    this.readingProgress = new Map(); // book title -> progress data
    this.readingLists = new Map(); // list name -> array of books

    this.setupToolHandlers();
    this.setupErrorHandling();
  }

  setupErrorHandling() {
    this.server.onerror = (error) => {
      console.error('[MCP Error]', error);
    };

    process.on('SIGINT', async () => {
      await this.server.close();
      process.exit(0);
    });
  }

  setupToolHandlers() {
    this.server.setRequestHandler(ListToolsRequestSchema, async () => {
      return {
        tools: [
          {
            name: 'play_audiobook',
            description: 'Play a specific audiobook by title, author, or search query',
            inputSchema: {
              type: 'object',
              properties: {
                query: {
                  type: 'string',
                  description: 'Book title, author, or search query (e.g., "1984", "George Orwell", "dystopian fiction")',
                },
                author: {
                  type: 'string',
                  description: 'Optional: Specific author name',
                },
                genre: {
                  type: 'string',
                  description: 'Optional: Book genre (e.g., "fiction", "non-fiction", "mystery", "sci-fi", "biography")',
                },
              },
              required: ['query'],
            },
          },
          {
            name: 'play_chapter',
            description: 'Play a specific chapter of the current audiobook',
            inputSchema: {
              type: 'object',
              properties: {
                chapter: {
                  type: 'number',
                  description: 'Chapter number to play',
                },
                book_title: {
                  type: 'string',
                  description: 'Optional: Book title if not currently loaded',
                },
              },
              required: ['chapter'],
            },
          },
          {
            name: 'control_playback',
            description: 'Control audiobook playback (play, pause, stop, next chapter, previous chapter, speed, seek)',
            inputSchema: {
              type: 'object',
              properties: {
                action: {
                  type: 'string',
                  enum: ['play', 'pause', 'stop', 'next_chapter', 'previous_chapter', 'speed', 'seek'],
                  description: 'Control action to perform',
                },
                value: {
                  type: 'number',
                  description: 'For speed: 0.5-3.0, for seek: position in seconds',
                },
              },
              required: ['action'],
            },
          },
          {
            name: 'search_audiobooks',
            description: 'Search for audiobooks by title, author, genre, or keywords',
            inputSchema: {
              type: 'object',
              properties: {
                query: {
                  type: 'string',
                  description: 'Search query for audiobooks',
                },
                type: {
                  type: 'string',
                  enum: ['title', 'author', 'genre', 'all'],
                  description: 'Type of search to perform',
                  default: 'all',
                },
                limit: {
                  type: 'number',
                  description: 'Maximum number of results to return',
                  default: 10,
                },
              },
              required: ['query'],
            },
          },
          {
            name: 'get_current_book',
            description: 'Get information about the currently playing audiobook and reading progress',
            inputSchema: {
              type: 'object',
              properties: {},
            },
          },
          {
            name: 'add_bookmark',
            description: 'Add a bookmark at the current position or specific time',
            inputSchema: {
              type: 'object',
              properties: {
                name: {
                  type: 'string',
                  description: 'Bookmark name or description',
                },
                position: {
                  type: 'number',
                  description: 'Optional: Position in seconds (if not provided, uses current position)',
                },
                chapter: {
                  type: 'number',
                  description: 'Optional: Chapter number',
                },
              },
              required: ['name'],
            },
          },
          {
            name: 'list_bookmarks',
            description: 'List all bookmarks for the current audiobook',
            inputSchema: {
              type: 'object',
              properties: {
                book_title: {
                  type: 'string',
                  description: 'Optional: Book title (if not provided, uses current book)',
                },
              },
            },
          },
          {
            name: 'set_reading_speed',
            description: 'Set the playback speed for audiobook reading',
            inputSchema: {
              type: 'object',
              properties: {
                speed: {
                  type: 'number',
                  description: 'Playback speed (0.5 = half speed, 1.0 = normal, 2.0 = double speed, max 3.0)',
                },
              },
              required: ['speed'],
            },
          },
          {
            name: 'get_reading_progress',
            description: 'Get detailed reading progress and statistics',
            inputSchema: {
              type: 'object',
              properties: {
                book_title: {
                  type: 'string',
                  description: 'Optional: Book title (if not provided, uses current book)',
                },
              },
            },
          },
          {
            name: 'create_reading_list',
            description: 'Create a reading list with multiple audiobooks',
            inputSchema: {
              type: 'object',
              properties: {
                name: {
                  type: 'string',
                  description: 'Reading list name',
                },
                books: {
                  type: 'array',
                  items: { type: 'string' },
                  description: 'Array of book titles or search queries',
                },
              },
              required: ['name', 'books'],
            },
          },
        ],
      };
    });

    this.server.setRequestHandler(CallToolRequestSchema, async (request) => {
      const { name, arguments: args } = request.params;

      try {
        switch (name) {
          case 'play_audiobook':
            return await this.playAudiobook(args);
          case 'play_chapter':
            return await this.playChapter(args);
          case 'control_playback':
            return await this.controlPlayback(args);
          case 'search_audiobooks':
            return await this.searchAudiobooks(args);
          case 'get_current_book':
            return await this.getCurrentBook(args);
          case 'add_bookmark':
            return await this.addBookmark(args);
          case 'list_bookmarks':
            return await this.listBookmarks(args);
          case 'set_reading_speed':
            return await this.setReadingSpeed(args);
          case 'get_reading_progress':
            return await this.getReadingProgress(args);
          case 'create_reading_list':
            return await this.createReadingList(args);
          default:
            throw new Error(`Unknown tool: ${name}`);
        }
      } catch (error) {
        return {
          content: [
            {
              type: 'text',
              text: `Error: ${error.message}`,
            },
          ],
          isError: true,
        };
      }
    });
  }

  async playAudiobook(args) {
    const { query, author, genre } = args;
    
    try {
      // Mock audiobook database
      const audiobookDatabase = [
        {
          title: '1984',
          author: 'George Orwell',
          genre: 'Dystopian Fiction',
          narrator: 'Simon Prebble',
          duration: 11 * 3600 + 30 * 60, // 11 hours 30 minutes
          chapters: 23,
          year: 1949,
          description: 'A dystopian social science fiction novel about totalitarian control.',
          url: 'mock://1984-audiobook.mp3'
        },
        {
          title: 'The Great Gatsby',
          author: 'F. Scott Fitzgerald',
          genre: 'Fiction',
          narrator: 'Jake Gyllenhaal',
          duration: 4 * 3600 + 49 * 60, // 4 hours 49 minutes
          chapters: 9,
          year: 1925,
          description: 'A story of the fabulously wealthy Jay Gatsby and his love for Daisy Buchanan.',
          url: 'mock://great-gatsby-audiobook.mp3'
        },
        {
          title: 'To Kill a Mockingbird',
          author: 'Harper Lee',
          genre: 'Fiction',
          narrator: 'Sissy Spacek',
          duration: 12 * 3600 + 17 * 60, // 12 hours 17 minutes
          chapters: 31,
          year: 1960,
          description: 'A story of racial injustice and childhood innocence.',
          url: 'mock://to-kill-mockingbird-audiobook.mp3'
        },
        {
          title: 'Dune',
          author: 'Frank Herbert',
          genre: 'Science Fiction',
          narrator: 'Scott Brick',
          duration: 21 * 3600 + 2 * 60, // 21 hours 2 minutes
          chapters: 48,
          year: 1965,
          description: 'An epic science fiction novel set on the desert planet Arrakis.',
          url: 'mock://dune-audiobook.mp3'
        },
        {
          title: 'The Martian',
          author: 'Andy Weir',
          genre: 'Science Fiction',
          narrator: 'R.C. Bray',
          duration: 10 * 3600 + 53 * 60, // 10 hours 53 minutes
          chapters: 26,
          year: 2011,
          description: 'A story of an astronaut stranded on Mars.',
          url: 'mock://martian-audiobook.mp3'
        },
        {
          title: 'Becoming',
          author: 'Michelle Obama',
          genre: 'Biography',
          narrator: 'Michelle Obama',
          duration: 19 * 3600 + 3 * 60, // 19 hours 3 minutes
          chapters: 24,
          year: 2018,
          description: 'A memoir by former First Lady Michelle Obama.',
          url: 'mock://becoming-audiobook.mp3'
        }
      ];

      // Search for matching audiobooks
      let results = audiobookDatabase.filter(book => {
        const queryLower = query.toLowerCase();
        const matchesQuery = book.title.toLowerCase().includes(queryLower) ||
                           book.author.toLowerCase().includes(queryLower) ||
                           book.description.toLowerCase().includes(queryLower);
        
        const matchesAuthor = !author || book.author.toLowerCase().includes(author.toLowerCase());
        const matchesGenre = !genre || book.genre.toLowerCase().includes(genre.toLowerCase());
        
        return matchesQuery && matchesAuthor && matchesGenre;
      });

      if (results.length === 0) {
        return {
          content: [
            {
              type: 'text',
              text: `📚 No audiobooks found matching "${query}". Try a different search term.`,
            },
          ],
        };
      }

      // Play the first matching audiobook
      const book = results[0];
      this.currentBook = book;
      this.currentChapter = 1;
      this.isPlaying = true;
      this.currentPosition = 0;

      // Initialize reading progress if not exists
      if (!this.readingProgress.has(book.title)) {
        this.readingProgress.set(book.title, {
          totalDuration: book.duration,
          completedDuration: 0,
          chaptersCompleted: 0,
          lastPosition: 0,
          startDate: new Date().toISOString()
        });
      }

      let result = `📚 Now Playing: "${book.title}" by ${book.author}\n`;
      result += `🎭 Narrator: ${book.narrator}\n`;
      result += `📖 Genre: ${book.genre}\n`;
      result += `📅 Year: ${book.year}\n`;
      result += `📄 Chapters: ${book.chapters}\n`;
      result += `⏱️ Duration: ${Math.floor(book.duration / 3600)}h ${Math.floor((book.duration % 3600) / 60)}m\n`;
      result += `📝 Description: ${book.description}\n\n`;
      result += `🎧 Chapter: ${this.currentChapter}/${book.chapters}\n`;
      result += `▶️ Playing: ${this.isPlaying ? 'YES' : 'NO'}\n`;
      result += `⚡ Speed: ${this.playbackSpeed}x\n`;
      result += `📍 Position: ${Math.floor(this.currentPosition / 60)}:${(this.currentPosition % 60).toString().padStart(2, '0')}\n\n`;

      if (results.length > 1) {
        result += `💡 Found ${results.length - 1} more matching audiobooks. Use "search_audiobooks" to see all results.`;
      }

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to play audiobook: ${error.message}`);
    }
  }

  async playChapter(args) {
    const { chapter, book_title } = args;
    
    try {
      let book = this.currentBook;
      
      if (book_title && book_title !== this.currentBook?.title) {
        // Search for the specified book
        const audiobookDatabase = [
          { title: '1984', author: 'George Orwell', chapters: 23 },
          { title: 'The Great Gatsby', author: 'F. Scott Fitzgerald', chapters: 9 },
          { title: 'To Kill a Mockingbird', author: 'Harper Lee', chapters: 31 },
          { title: 'Dune', author: 'Frank Herbert', chapters: 48 },
          { title: 'The Martian', author: 'Andy Weir', chapters: 26 },
          { title: 'Becoming', author: 'Michelle Obama', chapters: 24 }
        ];
        
        book = audiobookDatabase.find(b => b.title.toLowerCase().includes(book_title.toLowerCase()));
        if (!book) {
          return {
            content: [
              {
                type: 'text',
                text: `📚 Book "${book_title}" not found. Please load it first with "play_audiobook".`,
              },
            ],
          };
        }
      }

      if (!book) {
        return {
          content: [
            {
              type: 'text',
              text: '📚 No audiobook currently loaded. Use "play_audiobook" to load a book first.',
            },
          ],
        };
      }

      if (chapter < 1 || chapter > book.chapters) {
        return {
          content: [
            {
              type: 'text',
              text: `📚 Chapter ${chapter} not found. This book has ${book.chapters} chapters.`,
            },
          ],
        };
      }

      this.currentChapter = chapter;
      this.currentPosition = 0; // Reset position for new chapter
      this.isPlaying = true;

      let result = `📚 Playing Chapter ${chapter} of "${book.title}"\n`;
      result += `👤 Author: ${book.author}\n`;
      result += `📄 Total Chapters: ${book.chapters}\n`;
      result += `▶️ Status: Playing\n`;
      result += `⚡ Speed: ${this.playbackSpeed}x\n`;
      result += `📍 Position: 0:00\n\n`;
      result += `💡 Use "control_playback" to pause, seek, or change chapters.`;

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to play chapter: ${error.message}`);
    }
  }

  async controlPlayback(args) {
    const { action, value } = args;
    
    try {
      if (!this.currentBook) {
        return {
          content: [
            {
              type: 'text',
              text: '📚 No audiobook currently loaded. Use "play_audiobook" to load a book first.',
            },
          ],
        };
      }

      let result = '';
      
      switch (action) {
        case 'play':
          this.isPlaying = true;
          result = `▶️ Audiobook resumed`;
          if (this.currentBook) {
            result += ` - Now playing: "${this.currentBook.title}" Chapter ${this.currentChapter}`;
          }
          break;
          
        case 'pause':
          this.isPlaying = false;
          result = `⏸️ Audiobook paused`;
          break;
          
        case 'stop':
          this.isPlaying = false;
          this.currentPosition = 0;
          result = `⏹️ Audiobook stopped`;
          break;
          
        case 'next_chapter':
          if (this.currentChapter < this.currentBook.chapters) {
            this.currentChapter++;
            this.currentPosition = 0;
            result = `⏭️ Next chapter: Chapter ${this.currentChapter} of "${this.currentBook.title}"`;
          } else {
            result = `⏭️ Already at the last chapter (${this.currentBook.chapters})`;
          }
          break;
          
        case 'previous_chapter':
          if (this.currentChapter > 1) {
            this.currentChapter--;
            this.currentPosition = 0;
            result = `⏮️ Previous chapter: Chapter ${this.currentChapter} of "${this.currentBook.title}"`;
          } else {
            result = `⏮️ Already at the first chapter`;
          }
          break;
          
        case 'speed':
          if (value !== undefined && value >= 0.5 && value <= 3.0) {
            this.playbackSpeed = value;
            result = `⚡ Playback speed set to ${this.playbackSpeed}x`;
          } else {
            result = `⚡ Current speed: ${this.playbackSpeed}x (use value 0.5-3.0 to change)`;
          }
          break;
          
        case 'seek':
          if (value !== undefined) {
            this.currentPosition = value;
            result = `⏰ Seeked to ${Math.floor(value / 60)}:${(value % 60).toString().padStart(2, '0')}`;
          } else {
            result = `⏰ Use seek action with value parameter to seek to specific time in seconds`;
          }
          break;
          
        default:
          result = `❓ Unknown action: ${action}`;
      }

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to control playback: ${error.message}`);
    }
  }

  async searchAudiobooks(args) {
    const { query, type = 'all', limit = 10 } = args;
    
    try {
      const audiobookDatabase = [
        { title: '1984', author: 'George Orwell', genre: 'Dystopian Fiction', narrator: 'Simon Prebble' },
        { title: 'The Great Gatsby', author: 'F. Scott Fitzgerald', genre: 'Fiction', narrator: 'Jake Gyllenhaal' },
        { title: 'To Kill a Mockingbird', author: 'Harper Lee', genre: 'Fiction', narrator: 'Sissy Spacek' },
        { title: 'Dune', author: 'Frank Herbert', genre: 'Science Fiction', narrator: 'Scott Brick' },
        { title: 'The Martian', author: 'Andy Weir', genre: 'Science Fiction', narrator: 'R.C. Bray' },
        { title: 'Becoming', author: 'Michelle Obama', genre: 'Biography', narrator: 'Michelle Obama' },
        { title: 'Sapiens', author: 'Yuval Noah Harari', genre: 'Non-fiction', narrator: 'Derek Perkins' },
        { title: 'The Silent Patient', author: 'Alex Michaelides', genre: 'Mystery', narrator: 'Jack Hawkins' },
        { title: 'Educated', author: 'Tara Westover', genre: 'Biography', narrator: 'Julia Whelan' },
        { title: 'Atomic Habits', author: 'James Clear', genre: 'Self-help', narrator: 'James Clear' }
      ];

      const queryLower = query.toLowerCase();
      let results = audiobookDatabase.filter(item => {
        switch (type) {
          case 'title':
            return item.title.toLowerCase().includes(queryLower);
          case 'author':
            return item.author.toLowerCase().includes(queryLower);
          case 'genre':
            return item.genre.toLowerCase().includes(queryLower);
          default:
            return item.title.toLowerCase().includes(queryLower) ||
                   item.author.toLowerCase().includes(queryLower) ||
                   item.genre.toLowerCase().includes(queryLower) ||
                   item.narrator.toLowerCase().includes(queryLower);
        }
      }).slice(0, limit);

      if (results.length === 0) {
        return {
          content: [
            {
              type: 'text',
              text: `🔍 No audiobooks found for "${query}" in ${type} search.`,
            },
          ],
        };
      }

      let result = `🔍 Search results for "${query}" (${type}):\n\n`;
      results.forEach((item, index) => {
        result += `${index + 1}. 📚 ${item.title}\n`;
        result += `   👤 Author: ${item.author}\n`;
        result += `   🎭 Narrator: ${item.narrator}\n`;
        result += `   📖 Genre: ${item.genre}\n\n`;
      });

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to search audiobooks: ${error.message}`);
    }
  }

  async getCurrentBook(args) {
    try {
      if (!this.currentBook) {
        return {
          content: [
            {
              type: 'text',
              text: '📚 No audiobook currently playing. Use "play_audiobook" to start playing an audiobook.',
            },
          ],
        };
      }

      let result = `📚 Currently Playing:\n`;
      result += `📖 Title: "${this.currentBook.title}"\n`;
      result += `👤 Author: ${this.currentBook.author}\n`;
      result += `🎭 Narrator: ${this.currentBook.narrator}\n`;
      result += `📖 Genre: ${this.currentBook.genre}\n`;
      result += `📅 Year: ${this.currentBook.year}\n`;
      result += `📄 Total Chapters: ${this.currentBook.chapters}\n\n`;
      
      result += `🎧 Current Status:\n`;
      result += `📄 Chapter: ${this.currentChapter}/${this.currentBook.chapters}\n`;
      result += `▶️ Playing: ${this.isPlaying ? 'YES' : 'NO'}\n`;
      result += `⚡ Speed: ${this.playbackSpeed}x\n`;
      result += `📍 Position: ${Math.floor(this.currentPosition / 60)}:${(this.currentPosition % 60).toString().padStart(2, '0')}\n`;
      
      // Get reading progress
      const progress = this.readingProgress.get(this.currentBook.title);
      if (progress) {
        const progressPercent = Math.round((progress.completedDuration / progress.totalDuration) * 100);
        result += `📊 Progress: ${progressPercent}% completed\n`;
        result += `⏱️ Total Duration: ${Math.floor(progress.totalDuration / 3600)}h ${Math.floor((progress.totalDuration % 3600) / 60)}m\n`;
        result += `✅ Completed: ${Math.floor(progress.completedDuration / 3600)}h ${Math.floor((progress.completedDuration % 3600) / 60)}m`;
      }

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to get current book info: ${error.message}`);
    }
  }

  async addBookmark(args) {
    const { name, position, chapter } = args;
    
    try {
      if (!this.currentBook) {
        return {
          content: [
            {
              type: 'text',
              text: '📚 No audiobook currently loaded. Use "play_audiobook" to load a book first.',
            },
          ],
        };
      }

      const bookmarkPosition = position !== undefined ? position : this.currentPosition;
      const bookmarkChapter = chapter !== undefined ? chapter : this.currentChapter;

      if (!this.bookmarks.has(this.currentBook.title)) {
        this.bookmarks.set(this.currentBook.title, []);
      }

      const bookmark = {
        name,
        position: bookmarkPosition,
        chapter: bookmarkChapter,
        timestamp: new Date().toISOString(),
        timeString: `${Math.floor(bookmarkPosition / 60)}:${(bookmarkPosition % 60).toString().padStart(2, '0')}`
      };

      this.bookmarks.get(this.currentBook.title).push(bookmark);

      let result = `🔖 Bookmark added: "${name}"\n`;
      result += `📚 Book: ${this.currentBook.title}\n`;
      result += `📄 Chapter: ${bookmarkChapter}\n`;
      result += `⏰ Position: ${bookmark.timeString}\n`;
      result += `📅 Date: ${new Date(bookmark.timestamp).toLocaleDateString()}`;

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to add bookmark: ${error.message}`);
    }
  }

  async listBookmarks(args) {
    const { book_title } = args;
    
    try {
      const targetBook = book_title || this.currentBook?.title;
      
      if (!targetBook) {
        return {
          content: [
            {
              type: 'text',
              text: '📚 No book specified and no audiobook currently loaded.',
            },
          ],
        };
      }

      const bookmarks = this.bookmarks.get(targetBook) || [];

      if (bookmarks.length === 0) {
        return {
          content: [
            {
              type: 'text',
              text: `🔖 No bookmarks found for "${targetBook}". Use "add_bookmark" to create bookmarks.`,
            },
          ],
        };
      }

      let result = `🔖 Bookmarks for "${targetBook}":\n\n`;
      bookmarks.forEach((bookmark, index) => {
        result += `${index + 1}. 📌 ${bookmark.name}\n`;
        result += `   📄 Chapter: ${bookmark.chapter}\n`;
        result += `   ⏰ Position: ${bookmark.timeString}\n`;
        result += `   📅 Date: ${new Date(bookmark.timestamp).toLocaleDateString()}\n\n`;
      });

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to list bookmarks: ${error.message}`);
    }
  }

  async setReadingSpeed(args) {
    const { speed } = args;
    
    try {
      if (speed < 0.5 || speed > 3.0) {
        return {
          content: [
            {
              type: 'text',
              text: '⚡ Speed must be between 0.5x and 3.0x. Current speed: ' + this.playbackSpeed + 'x',
            },
          ],
        };
      }

      this.playbackSpeed = speed;

      let result = `⚡ Reading speed set to ${this.playbackSpeed}x\n`;
      
      if (this.currentBook) {
        result += `📚 Book: ${this.currentBook.title}\n`;
        result += `📄 Chapter: ${this.currentChapter}`;
      }

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to set reading speed: ${error.message}`);
    }
  }

  async getReadingProgress(args) {
    const { book_title } = args;
    
    try {
      const targetBook = book_title || this.currentBook?.title;
      
      if (!targetBook) {
        return {
          content: [
            {
              type: 'text',
              text: '📚 No book specified and no audiobook currently loaded.',
            },
          ],
        };
      }

      const progress = this.readingProgress.get(targetBook);

      if (!progress) {
        return {
          content: [
            {
              type: 'text',
              text: `📊 No reading progress found for "${targetBook}". Start reading to track progress.`,
            },
          ],
        };
      }

      const progressPercent = Math.round((progress.completedDuration / progress.totalDuration) * 100);
      const remainingDuration = progress.totalDuration - progress.completedDuration;
      const startDate = new Date(progress.startDate);
      const daysReading = Math.ceil((new Date() - startDate) / (1000 * 60 * 60 * 24));

      let result = `📊 Reading Progress for "${targetBook}":\n\n`;
      result += `📈 Overall Progress: ${progressPercent}%\n`;
      result += `⏱️ Total Duration: ${Math.floor(progress.totalDuration / 3600)}h ${Math.floor((progress.totalDuration % 3600) / 60)}m\n`;
      result += `✅ Completed: ${Math.floor(progress.completedDuration / 3600)}h ${Math.floor((progress.completedDuration % 3600) / 60)}m\n`;
      result += `⏳ Remaining: ${Math.floor(remainingDuration / 3600)}h ${Math.floor((remainingDuration % 3600) / 60)}m\n`;
      result += `📄 Chapters Completed: ${progress.chaptersCompleted}\n`;
      result += `📅 Started: ${startDate.toLocaleDateString()}\n`;
      result += `📆 Days Reading: ${daysReading} days\n`;
      
      if (daysReading > 0) {
        const avgPerDay = Math.round(progress.completedDuration / daysReading / 60); // minutes per day
        result += `📊 Average per Day: ${avgPerDay} minutes`;
      }

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to get reading progress: ${error.message}`);
    }
  }

  async createReadingList(args) {
    const { name, books } = args;
    
    try {
      this.readingLists.set(name, books);

      let result = `📚 Reading List Created: "${name}"\n`;
      result += `📋 ${books.length} books added:\n\n`;
      
      books.forEach((book, index) => {
        result += `${index + 1}. 📖 ${book}\n`;
      });

      result += `\n💡 Use "play_audiobook" to start reading any book from this list.`;

      return {
        content: [
          {
            type: 'text',
            text: result,
          },
        ],
      };
    } catch (error) {
      throw new Error(`Failed to create reading list: ${error.message}`);
    }
  }

  async run() {
    try {
      const transport = new StdioServerTransport();
      await this.server.connect(transport);
      console.error('Audiobook MCP server running on stdio');
    } catch (error) {
      console.error('Failed to start server:', error);
      process.exit(1);
    }
  }
}

const server = new AudiobookMCPServer();
server.run().catch(console.error);
