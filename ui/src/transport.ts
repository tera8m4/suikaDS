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

    const socket = new WebSocket("ws://localhost:3333");
    socket.addEventListener("message", (event) => {
      const data = JSON.parse(event.data);
      if (data["method"] == "initialize") {
        this.initializeHandlers.forEach(x => x.callback.call(x.owner, data));
      }
      else {
        this.messageHandlers.forEach(x => x.callback.call(x.owner, data));
      }

    });
    socket.addEventListener("error", () => {
      if (this.timerID == undefined) {
        this.timerID = setTimeout(this.connect.bind(this), 2000);
      }
    });
    socket.addEventListener("open", () => {
      this.initializeHandlers.forEach(x => x.callback.call(x.owner, { title: "connected!" }));
    });
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
