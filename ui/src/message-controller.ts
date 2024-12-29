import { ReactiveController, ReactiveControllerHost } from "lit";
import { Transport, TransportMessage } from "./transport";

export type CardMessage = TransportMessage & {
  id: number;
}

export class MessageController implements ReactiveController {
  private static MAX_MESSAGES = 100;

  host: ReactiveControllerHost;

  message: CardMessage[] = [];
  private counter: number = 0;

  constructor(host: ReactiveControllerHost) {
    this.host = host;
    this.host.addController(this);
  }

  hostConnected(): void {
    Transport.getInstance().addMessageHandler(this, this.onMessageReceived);
  }

  hostDisconnected(): void {
    Transport.getInstance().removeAll(this);
  }

  private onMessageReceived(message: TransportMessage): void {
    this.message.push({ ...message, id: this.counter });
    ++this.counter;

    while (this.message.length > MessageController.MAX_MESSAGES) {
      this.message.shift();
    }

    this.host.requestUpdate();
    setTimeout(() => {
      window.scrollTo({
        top: document.documentElement.scrollHeight,
        behavior: 'smooth' // Optional: Adds smooth scrolling
      });
    }, 100);
  }
}
