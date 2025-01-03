export type TransportMessage = {
  message: string;
  image?: string;
};

export type TransportInitialized = {
  title: string;
}

type MessageCallback = (message: TransportMessage) => void;
type InitializeCallback = (data: TransportInitialized) => void;

export class Transport {
  public static getInstance(): Transport {
    if (this.instance == undefined) {
      this.instance = new Transport();
    }

    return this.instance;
  }


  private static instance?: Transport;
  private messageHandlers: { owner: any, callback: MessageCallback }[] = [];
  private initializeHandlers: { owner: any, callback: InitializeCallback }[] = [];
  private initialized: boolean = false;
  private timerID?: number = undefined;
  private socket!: WebSocket;
  private updateScreenshotCallback?: (value: string) => void = undefined;

  private constructor() {
  }

  public init(): void {
    if (!this.initialized) {
      this.connect();
    }
    this.initialized = true;
  }

  private connect(): void {
    if (this.timerID !== undefined) {
      clearTimeout(this.timerID);
    }

    this.socket = new WebSocket("ws://localhost:3333");
    this.socket.addEventListener("message", (event) => {
      const data = JSON.parse(event.data);
      if (data["method"] == "initialize") {
        this.initializeHandlers.forEach(x => x.callback.call(x.owner, data));
      }
      else if (data["method"] == "update_screenshot") {
        if (this.updateScreenshotCallback !== undefined) {
          this.updateScreenshotCallback(data["image"]);
          this.updateScreenshotCallback = undefined;
        }
      }
      else {
        this.messageHandlers.forEach(x => x.callback.call(x.owner, data));
      }

    });
    this.socket.addEventListener("error", () => {
      if (this.timerID == undefined) {
        this.timerID = setTimeout(this.connect.bind(this), 2000);
      }
    });
    this.socket.addEventListener("open", () => {
      this.initializeHandlers.forEach(x => x.callback.call(x.owner, { title: "connected!" }));
    });
  }

  public requestNewScreenshot(): Promise<string> {
    if (this.updateScreenshotCallback !== undefined) {
      return Promise.reject();
    }

    const promise = new Promise<string>((resolver) => {
      this.updateScreenshotCallback = resolver;
    })
    this.socket.send("update_screenshot");

    return promise;
  }

  public addMessageHandler(owner: any, callback: MessageCallback): void {
    this.messageHandlers.push({ owner, callback });
  }

  public addInitializeHandler(owner: any, callback: InitializeCallback): void {
    this.initializeHandlers.push({ owner, callback });
  }

  public removeAll(owner: any): void {
    this.messageHandlers = this.messageHandlers.filter(x => x.owner != owner);
    this.initializeHandlers = this.initializeHandlers.filter(x => x.owner != owner);
  }
}
